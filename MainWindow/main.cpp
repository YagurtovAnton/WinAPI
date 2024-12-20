#include<Windows.h>
#include<iostream>
#include<cstdio>
#include"resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Window";	//��� ������ ����

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) ����������� ������ ����:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.style = 0;
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;	//cb... - Count Bytes

	wc.hIcon = (HICON)LoadImage(NULL,"ico\\bitcoin.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(NULL,"ico\\doom.ico",IMAGE_ICON,LR_DEFAULTSIZE,LR_DEFAULTSIZE,LR_LOADFROMFILE);
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "cursor\\w3-undead\\HELP.ani",
		IMAGE_CURSOR,
		LR_DEFAULTSIZE,
		LR_DEFAULTSIZE,
		LR_LOADFROMFILE
	);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;


	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_MY_WINDOW_CLASS;
	wc.lpfnWndProc = (WNDPROC)WndProc;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) ��������� ����:
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	INT screen_width = GetSystemMetrics(SM_CXSCREEN);
	INT screen_heght = GetSystemMetrics(SM_CYSCREEN);

	printf("Screen:\t %ix%i;\n", screen_width, screen_heght);

	INT window_width = screen_width * 1.7/ 4;
	INT window_heght = screen_heght * .55;
	printf("Window:\t %ix%i;\n", window_width, window_heght);
	INT window_start_x = screen_width / 2;
	INT window_start_y = screen_heght / 8;
	printf("Location:\t %ix%i;\n", window_start_x, window_start_y);

	HWND hwnd = CreateWindowEx
	(
		NULL,		//ExStyles
		g_sz_MY_WINDOW_CLASS,	//Class name
		g_sz_MY_WINDOW_CLASS,	//Window title
		WS_OVERLAPPEDWINDOW,	//Window style
		window_start_x, window_start_y,	//Window position
		window_width, window_heght,	//Window size
		NULL,	//Parent Window
		NULL,	//Main menu ResourceID for MainWindow or ResourceID for ChildWindow
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);	//������ ����� ����������� ���� 
	//(���������� �� ���� �����, �������� � ����, �������� � ������ �����)
	UpdateWindow(hwnd);			//������������� ����.

	//3) ������ ����� ���������:
	MSG msg;	//������� ���������
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);
		IsDialogMessage(hwnd, &msg);//TAB - ������������ ������
	}

	return msg.message;
}
//
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		CreateWindowEx
		(
			NULL,
			"Static",
			"��� ������� ������� ��������� CreatWindowsEx()",
			WS_CHILDWINDOW | WS_VISIBLE|WS_TABSTOP,
			10, 10,
			500, 25,
			hwnd,
			(HMENU)IDC_STATIC,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Edit",
			"",
			WS_CHILD | WS_VISIBLE | WS_BORDER|WS_TABSTOP,
			10, 48,
			420, 25,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL,
			"Button",
			"���������",
			WS_CHILD | WS_VISIBLE| WS_TABSTOP,
			275, 85,
			150, 32,
			hwnd,
			(HMENU)IDC_BATTON,
			GetModuleHandle(NULL),
			NULL
		);
		break;
	case WM_MOVE:
	case WM_SIZE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		INT window_width = rect.right - rect.left;
		INT window_heght = rect.bottom - rect.top;
		CONST INT SIZE = 256;
		CHAR sz_title[SIZE]{};
		sprintf
		(
			sz_title,
			"%s - Position: %ixi%; Size: %ix%i",
			g_sz_MY_WINDOW_CLASS,
			rect.left, rect.top,
			window_width,window_heght
		);

		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
		printf(sz_title);
		printf("\n");
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BATTON:

			CONST INT SIZE = 256;
			CHAR cz_buffer[SIZE]{};
			HWND hStatic = GetDlgItem(hwnd, IDC_STATIC);
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)cz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)cz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)cz_buffer);
			SendMessage(GetDlgItem(hwnd,IDC_BATTON), WM_SETTEXT, 0, (LPARAM)cz_buffer);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}