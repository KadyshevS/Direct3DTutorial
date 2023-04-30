#include "KDWindow.h"

#include <sstream>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	try
	{
		KDE::KDWindow window{ "Test KDWindow", 800, 600 };

		MSG msg;
		BOOL gResult;

		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			static int i = 0;
			while (!window.Mouse.IsEmpty())
			{
				const auto e = window.Mouse.Read();
				switch (e.Type())
				{
					case KDE::KDMouse::Event::EventType::WheelUp:
						i++;
						{
							std::ostringstream oss;
							oss << "Wheel Up: " << i;
							window.SetTitle(oss.str().c_str());
						}
						break;
					case KDE::KDMouse::Event::EventType::WheelDown:
						i--;
						{
							std::ostringstream oss;
							oss << "Wheel Down: " << i;
							window.SetTitle(oss.str().c_str());
						}
						break;
				}
			}
		}

		return (int)msg.wParam;
	}
	catch (const KDE::KDException& e)
	{
		MessageBox(nullptr, e.what(), e.Type(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}