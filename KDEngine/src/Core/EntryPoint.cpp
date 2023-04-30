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

			while (!window.Mouse.IsEmpty())
			{
				const auto e = window.Mouse.Read();
				if (e.Type() == KDE::KDMouse::Event::EventType::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position: {" << e.PositionX()
						<< ", " << e.PositionY() << "}";
					window.SetTitle(oss.str().c_str());
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