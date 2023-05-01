#pragma once

#include "KDWin.h"
#include "KDException.h"
#include "Input/KDKeyboard.h"
#include "Input/KDMouse.h"
#include "Graphics/KDGraphics.h"

#include <optional>
#include <memory>

namespace KDE
{
	class KDWindow
	{
	public:
		KDWindow(const char* title, int width, int height);
		KDWindow(const KDWindow&) = delete;
		KDWindow& operator = (const KDWindow&) = delete;
		~KDWindow();

		void SetTitle(const char* title);
		static std::optional<int> ProcessMessages();
	public:
		class Exception : public KDException
		{
		public:
			Exception(int line, const char* file, HRESULT hr);

			const char* what() const override;
			virtual const char* Type() const override;

			static std::string TranslateErrorCode(HRESULT hr);
			HRESULT ErrorCode() const;
			std::string ErrorString() const;  
		private:
			HRESULT m_ErrorCode;
		};

		const char* Title() const;
		int Width() const;
		int Height() const;
		KDGraphics& Graphics();

		KDKeyboard Keyboard;
		KDMouse Mouse;

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
		std::string m_Title = "Unknown Title";
		int m_Width = 800, m_Height = 600;
		HWND m_WindowHandle;
		std::unique_ptr<KDGraphics> m_Graphics;
	};
}

#define KD_EXCEPT(hr) KDWindow::Exception(__LINE__, __FILE__, hr)
#define KD_EXCEPT_LAST() KDWindow::Exception(__LINE__, __FILE__, GetLastError())