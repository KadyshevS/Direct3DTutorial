#include "Core/KDApplication.h"

extern KDE::KDApplication* KDE::CreateApplication();

int CALLBACK WinMain(
	_In_	 HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_	 LPSTR lpCmdLine,
	_In_	 int nCmdShow
)
{
	try
	{
		auto app = KDE::CreateApplication();
		app->Run();
		delete app;

		return 0;
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