#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd)
{
	try
	{
		Window wnd(800, 300, "Donkey Fart Box");

		// message pump
		MSG msg;
		BOOL bRet;
		while ((bRet = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (bRet == -1)
		{
			throw CHWND_LAST_EXCEPT();
		}
		return msg.wParam;
	}
	catch (const ChiliException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No detail available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	
	return -1;
}