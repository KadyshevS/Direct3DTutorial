#include "KDWindow.h"

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

			if (window.Keyboard.IsKeyPressed(KDE::Key::Space))
				MessageBox(nullptr, "Something happen", "Space key was pressed.", MB_OK);
			if (window.Keyboard.IsKeyPressed(KDE::Key::Control))
				MessageBox(nullptr, "Something happen", "Control key was pressed.", MB_OK);
			if (window.Keyboard.IsKeyPressed(KDE::Key::Shift))
				MessageBox(nullptr, "Something happen", "Shift key was pressed.", MB_OK);
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