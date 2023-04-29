#pragma once
#include "KDWin.h"

namespace KDE
{
	class KDWindow
	{
	public:
		KDWindow(const char* title, int width, int height);
		KDWindow(const KDWindow&) = delete;
		KDWindow& operator = (const KDWindow&) = delete;
		~KDWindow();

	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		class KDWindowClass
		{
		public:
			KDWindowClass();
			KDWindowClass(const KDWindow&) = delete;
			KDWindowClass& operator = (const KDWindowClass&) = delete;
			~KDWindowClass();

			static const char* Name();
			static HINSTANCE Instance();
		private:
			static constexpr const char* m_WindowClassName = "KDEngine Window Class";
			static KDWindowClass m_WindowClass;
			HINSTANCE m_Instance;
		};
		static constexpr int const m_Width = 800, m_Height = 600;
		HWND m_WindowHandle;
	};
}