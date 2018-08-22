#pragma once
#include<Windows.h>
#include<tchar.h>
#include<fstream>
#include<sstream>
#include<direct.h>
#include"Injector.h"

LRESULT CALLBACK WndProc(
	_In_ HWND	hwnd,
	_In_ UINT	uMsg,
	_In_ WPARAM	wParam,
	_In_ LPARAM	lParam
) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (uMsg) {
	case WM_PAINT: /*Here the application gets drawn*/
		hdc = BeginPaint(hwnd, &ps);
		/*Any drawing done here.*/
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
	case WM_NOTIFY:
		switch (wParam) {
		case BN_CLICKED:
			if (inject()) {
				MessageBox(
					NULL,
					_T("Injection failed!"),
					_T("DAPI Launcher"),
					NULL
				);
			}
			break;
		default:
			break;
		}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

int CALLBACK WinMain(
	_In_ HINSTANCE	hInstance,
	_In_ HINSTANCE	hPrevInstance,
	_In_ LPSTR		lpCmdLine,
	_In_ int		nCmdShow
) {
	/*Build the Window*/
	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = hInstance;
	window_class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = "Main";
	window_class.hIconSm = LoadIcon(window_class.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	/*Register the window with Windows*/
	if (!RegisterClassEx(&window_class)) {
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("DAPI Launcher"),
			NULL);

		return 1;
	}

	/*Create the window*/
	static TCHAR szWindowClass[] = _T("Main");
	static TCHAR szTitle[] = _T("DAPI Launcher");
	// The parameters to CreateWindow explained:  
	// szWindowClass: the name of the application  
	// szTitle: the text that appears in the title bar  
	// WS_OVERLAPPEDWINDOW: the type of window to create  
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
	// 500, 100: initial size (width, length)  
	// NULL: the parent of this window  
	// NULL: this application does not have a menu bar  
	// hInstance: the first parameter from WinMain  
	// NULL: not used in this application 
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		150, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd) {
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("DAPI Launcher"),
			NULL);

		return 1;
	}

	/*Build the Launch button*/
	HWND hwndButton = CreateWindow(
		_T("BUTTON"), /*Button class.*/
		_T("Launch Diablo"), /*Button text.*/
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, /*Styles*/
		10, /*x position.*/
		10, /*y position.*/
		100, /*Button width.*/
		20, /*Button height.*/
		hWnd, /*Parent window.*/
		NULL, /*No menu*/
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), /*hInstance of parent window.*/
		NULL); /*Pointer not needed.*/

	/*Make window visible*/
	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	/*Event loop*/
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}