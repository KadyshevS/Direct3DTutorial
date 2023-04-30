#include "KDWindow.h"

#include "resource.h"

#include <sstream>

namespace KDE
{
//////////////////////////////////////////////////////////////////////////
////	Window Class
	KDWindow::KDWindowClass KDWindow::KDWindowClass::m_WindowClass;

	KDWindow::KDWindowClass::KDWindowClass()
		: m_Instance(GetModuleHandle(nullptr))
	{
		WNDCLASSEX wc{};

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpfnWndProc = HandleMsgSetup;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = Instance();
		wc.hIcon = (HICON)(LoadImage(m_Instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
		wc.hCursor = nullptr;
		wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = m_WindowClassName;
		wc.hIconSm = (HICON)(LoadImage(m_Instance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));;
		RegisterClassEx(&wc);
	}

	KDWindow::KDWindowClass::~KDWindowClass()
	{
		UnregisterClass(m_WindowClassName, Instance());
	}
	const char* KDWindow::KDWindowClass::Name()
	{
		return m_WindowClassName;
	}
	HINSTANCE KDWindow::KDWindowClass::Instance()
	{
		return m_WindowClass.m_Instance;
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////	Window
	KDWindow::KDWindow(const char* title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		RECT wr{};
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
			throw KD_EXCEPT_LAST();

		m_WindowHandle = CreateWindow(
			KDWindowClass::Name(), title,
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, KDWindowClass::Instance(), this
		);

		if(m_WindowHandle == nullptr)
			throw KD_EXCEPT_LAST();

		ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
	}
	KDWindow::~KDWindow()
	{
		DestroyWindow(m_WindowHandle);
	}
	void KDWindow::SetTitle(const char* title)
	{
		if (SetWindowText(m_WindowHandle, title) == 0)
			throw KD_EXCEPT_LAST();
	}

	const char* KDWindow::Title() const
	{
		return m_Title.c_str();
	}
	int KDWindow::Width() const
	{
		return m_Width;
	}
	int KDWindow::Height() const
	{
		return m_Height;
	}

	LRESULT CALLBACK KDWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			KDWindow* const pWnd = static_cast<KDWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&KDWindow::HandleMsgThunk));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	LRESULT CALLBACK KDWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		KDWindow* const pWnd = reinterpret_cast<KDWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	LRESULT KDWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_KILLFOCUS:
				Keyboard.ClearState();
				break;

		//////////////////////////////////////////////////////////////////////////
		////	Keyboard Input
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				if( !(lParam & 0x40000000) || Keyboard.IsAutorepeatEnabled() )
					Keyboard.OnKeyPressed((unsigned char)wParam);
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				Keyboard.OnKeyReleased((unsigned char)wParam);
				break;
			case WM_CHAR:
				Keyboard.OnChar((unsigned char)wParam);
				break;
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		////	Mouse Input
			case WM_MOUSEMOVE:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				if (pt.x >= 0 && pt.x <= m_Width && pt.y >= 0 && pt.y <= m_Height)
				{
					Mouse.OnMove(pt.x, pt.y);
					if (!Mouse.IsInWindow())
					{
						SetCapture(hWnd);
						Mouse.OnEnter();
					}
				}
				else
				{
					if (wParam & (MK_LBUTTON | MK_RBUTTON))
					{
						Mouse.OnMove(pt.x, pt.y);
					}
					else
					{
						ReleaseCapture();
						Mouse.OnLeave();
					}
				}
				break;
			}
			case WM_LBUTTONDOWN:
				Mouse.OnLeftPressed();
				break;
			case WM_RBUTTONDOWN:
				Mouse.OnRightPressed();
				break;
			case WM_LBUTTONUP:
				Mouse.OnLeftReleased();
				break;
			case WM_RBUTTONUP:
				Mouse.OnRightPressed();
				break;
			case WM_MOUSEWHEEL:
			{
				const POINTS pt = MAKEPOINTS(lParam);
				if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
					Mouse.OnWheelUp(pt.x, pt.y);
				else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
					Mouse.OnWheelDown(pt.x, pt.y);
			}
				break;
		//////////////////////////////////////////////////////////////////////////
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////	Window Exception
	KDWindow::Exception::Exception(int line, const char* file, HRESULT hr)
		: KDException(line, file), m_ErrorCode(hr)
	{}
	
	const char* KDWindow::Exception::what() const
	{ 
		std::ostringstream oss;
		oss << Type() << '\n'
			<< "[Error Code] " << ErrorCode() << '\n'
			<< "[Description] " << ErrorString() << '\n'
			<< StringOrigin();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	const char* KDWindow::Exception::Type() const
	{
		return "KDWindow Exception";
	}
	std::string KDWindow::Exception::TranslateErrorCode(HRESULT hr)
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
		);
		if (nMsgLen == 0)
			return "Unidentified Error Code";

		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}
	HRESULT KDWindow::Exception::ErrorCode() const
	{
		return m_ErrorCode;
	}
	std::string KDWindow::Exception::ErrorString() const
	{
		return TranslateErrorCode(m_ErrorCode);
	}
//////////////////////////////////////////////////////////////////////////
}