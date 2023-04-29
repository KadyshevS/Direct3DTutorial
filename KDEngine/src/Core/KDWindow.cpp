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
	{
		RECT wr{};
		wr.left = 100;
		wr.right = width + wr.left;
		wr.top = 100;
		wr.bottom = height + wr.top;

		if ( FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)) )
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