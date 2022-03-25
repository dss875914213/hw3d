#include <windows.h>
#include "WindowsMessageMap.h"

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static WindowsMessageMap mm;
	OutputDebugString(mm(msg, lParam, wParam).c_str());
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wParam == 'D')
		{
			SetWindowText(hWnd, "D key down");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'F')
		{
			SetWindowText(hWnd, "F key up");
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd)
{
	const char* className = "hw3d";
	// register window class
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	RegisterClass(&wc);

	// create window instance
	HWND hWnd = CreateWindow(className, 
		"hw3d T2 Test", 
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 400, 300, 
		NULL,
		NULL, 
		hInstance,
		NULL);

	// show the window
	ShowWindow(hWnd, SW_SHOW);

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
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}