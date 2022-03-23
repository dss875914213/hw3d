#include <windows.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nShowCmd)
{
	const char* className = "hw3d";
	// register window class
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
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
		WS_CAPTION, 
		200, 200, 400, 300, 
		NULL,
		NULL, 
		hInstance,
		NULL);

	// show the window
	ShowWindow(hWnd, SW_SHOW);
	while (1);
}