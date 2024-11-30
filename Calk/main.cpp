#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<iostream>
#include"resource.h"


CONST CHAR g_sz_CLASS_NAME[] = "Calc PV_319";

CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 5;
CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_FONT_HEIGHT = 32;
CONST INT g_i_FONT_WIDTH = g_i_FONT_HEIGHT * 2 / 5;

CONST INT g_i_DISPLAY_WIDTH = g_i_BUTTON_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = g_i_FONT_HEIGHT + 2;

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;
CONST INT g_i_BUTTON_START_X = g_i_START_X;
CONST INT g_i_BUTTON_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;
CONST INT g_i_OPERETION_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;
CONST INT g_i_OPERETION_BUTTON_START_Y = g_i_BUTTON_START_Y;
CONST INT g_i_CONTROL_BUTTON_START_X = g_i_START_X + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4;
CONST INT g_i_CONTROL_BUTTON_START_Y = g_i_BUTTON_START_Y;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + 38;
CONST CHAR* g_OPERATIONS[] = { "+","-","*","/" };

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID  SetSkin(HWND hwnd, CONST CHAR* skin);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) 
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass));

	wClass.style = 0;
	wClass.cbSize = sizeof(wClass);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wClass.hInstance = hInstance;
	wClass.lpszClassName = g_sz_CLASS_NAME;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = NULL;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registertion failed", NULL, MB_OK | MB_ICONERROR);
		return 0;
	}
	//2)
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_CLASS_NAME,
		g_sz_CLASS_NAME,
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//3)
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DOUBLE a = DBL_MIN;
	static DOUBLE b = DBL_MIN;
	static INT operatoin;
	static BOOL input = FALSE;
	static BOOL input_opeatoin = FALSE;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
			10, 10,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_EDIT_DISPLAY,
			GetModuleHandle(NULL),
			NULL
		);
		AddFontResourceEx("Fonts\\Calculator.ttf", FR_PRIVATE, 0);
		HFONT hFont = CreateFont
		(
			g_i_FONT_HEIGHT, g_i_FONT_WIDTH,
			0,			//Escapement - Наклон шрифта в десятках градусов
			0,			//Orientation - ???
			FW_BOLD,	//Weight - Толщина
			FALSE,		//Italic - Курсив
			FALSE,		//Underline-Подчеркнутый
			FALSE,		//Strikeout - Перечеркнутый
			ANSI_CHARSET,
			OUT_TT_PRECIS,
			CLIP_TT_ALWAYS,
			ANTIALIASED_QUALITY,
			FF_DONTCARE,
			"Calculator"
		);
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
		//TODO:	Button Icons.

		CHAR sz_digit[2] = "0";
		for (int i = 6; i >= 0; i -= 3)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = 49 + i + j;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
					g_i_BUTTON_START_X + j * (g_i_BUTTON_SIZE + g_i_INTERVAL),
					g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (2 - i / 3),
					g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
					hwnd,
					(HMENU)(IDC_BUTTON_1 + i + j),
					GetModuleHandle(NULL),
					NULL
				);
			}

		}

		HWND hButton0 = CreateWindowEx
		(
			
			NULL, "Button", "0",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_BUTTON_START_X,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL
		);
		HBITMAP hBitmap0 = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			"ButtonBMP\\square_blue\\button_0.bmp",
			IMAGE_BITMAP,
			g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);
		//SendMessage(hButton0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap0);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON| BS_BITMAP,
			g_i_BUTTON_START_X + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);
		for (int i = 0; i < 4; i++)
		{
			CreateWindowEx
			(
				NULL, "Button", g_OPERATIONS[i],
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				g_i_OPERETION_BUTTON_START_X,
				g_i_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);

		}

		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y + g_i_BUTTON_SIZE + g_i_INTERVAL,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_CONTROL_BUTTON_START_X,
			g_i_CONTROL_BUTTON_START_Y + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 2,
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
		SetSkin(hwnd, "square_blue");
	}
	break;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		HWND hEdit = (HWND)lParam;
		if (GetDlgCtrlID(hEdit) == IDC_EDIT_DISPLAY)
		{
			SetTextColor(hdc, RGB(255, 0, 0));
			SetBkColor(hdc, RGB(0, 0, 100));
			HBRUSH hbrBackground = CreateSolidBrush(RGB(30, 30, 30));
			return (INT_PTR)hbrBackground;
		}
	}

	case WM_COMMAND:
	{
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		CHAR sz_digit[2] = {};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
		{
			//TODO ,,,,,,,,,,,,,,,,,,,,,,,,,,,
			if (!input && !input_opeatoin)
			{
				SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0);
				//ZeroMemory(sz_display, SIZE);
				sz_display[0] = 0;
			}
			if (!input && input_opeatoin/* && !strchr(sz_display, '.')*/)sz_display[0] = 0;

			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			if (strlen(sz_display) == 1 && sz_display[0] == '0')
				sz_display[0] = sz_digit[0];
			else
				strcat(sz_display, sz_digit);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);

			input = TRUE;
		//	input_opeatoin = FALSE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_POINT)
		{
			if (strchr(sz_display, '.')&& input)break;
			if (input_opeatoin && a == atof(sz_display))strcpy(sz_display, "0");
			else strcat(sz_display, ".");
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			strcat(sz_display, ".");
			input = TRUE;
			//input = TRUE;
		}
		//if (LOWORD(wParam) == IDC_EDIT_DISPLAY && HIWORD(wParam)==EN_SETFOCUS)SetFocus(hwnd);
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			if (strlen(sz_display) == 1)sz_display[0] = '0';
			else sz_display[strlen(sz_display) - 1] = 0;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLR)
		{
			a = b = DBL_MIN;
			operatoin = 0;
			input = FALSE;
			input_opeatoin = FALSE;
			sz_display[0] = 0;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		} 
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLAH)
		{
			//SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (a == DBL_MIN)a = atof(sz_display);
			//input = FALSE;
			if(input_opeatoin)SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			operatoin = LOWORD(wParam);
			input_opeatoin = TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			if (input || b == DBL_MIN && !input)b = atof(sz_display);
			input = FALSE;
			switch (operatoin)
			{
			case IDC_BUTTON_PLUS:	a += b; break;
			case IDC_BUTTON_MINUS:	a -= b; break;
			case IDC_BUTTON_ASTER:	a *= b; break;
			case IDC_BUTTON_SLAH:	a /= b; break;
			}
			input_opeatoin = FALSE;
			if (a == DBL_MIN)strcpy(sz_display, "0");
			else sprintf(sz_display, "%g", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		SetFocus(hwnd);
	}
	break;
	//ClassWork
	//1 при нажатие выбора темы закрыватся/завершается  калькулятор
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT< 0))
		{
			if (wParam == 0x38)SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
			
		}
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		}
		else if (wParam > 0x60 && wParam <= 0x69)
		{
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, TRUE, 0);
		}
		switch (wParam)
		{
			case VK_ADD:
			case VK_OEM_PLUS:
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_PLUS), BM_SETSTATE, TRUE, 0);break;
			case VK_SUBTRACT:
			case VK_OEM_MINUS:
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_MINUS), BM_SETSTATE, TRUE, 0);break;
			case VK_MULTIPLY:
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);break;
			case VK_DIVIDE:
			case VK_OEM_2:
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_SLAH), BM_SETSTATE,  TRUE, 0);break;
			case VK_DECIMAL:
			case VK_OEM_PERIOD:
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_POINT), BM_SETSTATE, TRUE, 0);
		}
		
	}
		break;
	case WM_KEYUP:
	{
		if (GetKeyState(VK_SHIFT) < 0)
		{
			if (wParam == 0x38)
			{
				SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
				SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_ASTER, 0);
			}
		}
	//	if (wParam >= 0x30 && wParam <= 0x39)
		//	SendMessage(hwnd, WM_COMMAND, wParam - 0x30 + IDC_BUTTON_0, 0);
		else if (wParam >= '0' && wParam <= '9')
		{
			SendMessage(GetDlgItem(hwnd, wParam - '0' + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, wParam - '0' + IDC_BUTTON_0, 0);
		}
		else if (wParam >= 0x60 && wParam <= 0x69)
			SendMessage(GetDlgItem(hwnd, wParam - 0x60 + IDC_BUTTON_0), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, wParam - 0x60 + IDC_BUTTON_0, 0);
	switch (wParam)
		{
		case	VK_ADD:
		case	VK_OEM_PLUS:	 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0); break;
		case	VK_SUBTRACT:
		case	VK_OEM_MINUS:	 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0); break;
		case	VK_MULTIPLY:	 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0); break;
		case	VK_DIVIDE:
		case	VK_OEM_2:		 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLAH), 0); break;
		case	VK_DECIMAL:
		case	VK_OEM_PERIOD:	 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0); break;
		case	VK_BACK:		 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0); break;
		case	VK_ESCAPE:		 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLR), 0); break;
		case	VK_RETURN:		 SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0); break;
		}
	}
	break;
	case WM_CONTEXTMENU:
	{
		HMENU hSubmenuSkins = CreatePopupMenu();
		InsertMenu(hSubmenuSkins, 0, MF_BYPOSITION | MF_STRING, IDR_METAL_MISTRAL, "Metal Mistral");
		InsertMenu(hSubmenuSkins, 0, MF_BYPOSITION | MF_STRING, IDR_SQUARE_BLUE, "Square Blue");

		HMENU hMenu = CreatePopupMenu();
		InsertMenu(hMenu, 0, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubmenuSkins, "Skins");
		InsertMenu(hMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
		InsertMenu(hMenu, 2, MF_BYPOSITION | MF_STRING, IDR_EXIT, "Exit");

		switch (TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_RETURNCMD, LOWORD(lParam), HIWORD(lParam), 0, hwnd, 0))
		{
		case IDR_SQUARE_BLUE:	SetSkin(hwnd, "square_blue"); break;
		case IDR_METAL_MISTRAL:	SetSkin(hwnd, "metal_mistral"); break;
		case IDR_EXIT:			DestroyWindow(hwnd);
		}
		DestroyMenu(hSubmenuSkins);
		DestroyMenu(hMenu);
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

}

CONST CHAR* g_BUTTON_FILENAME[] =
{
"button_0",
"button_1",
"button_2",
"button_3",
"button_4",
"button_5",
"button_6",
"button_7",
"button_8",
"button_9",
  
"button_point",
"button_plus",
"button_minus",
"button_aster",
"button_slash",
  
"button_bsp",
"button_clr",
"button_equal",
};

CHAR* FormatLastError()
{
	DWORD dwErrorMessageID = GetLastError();
	LPSTR lpszMessageBuffer = NULL;
	DWORD dwSize = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_ENGLISH_US),
		(LPSTR)&lpszMessageBuffer,
		0,
		NULL
	);
	return lpszMessageBuffer;
}

VOID SetSkin(HWND hwnd, CONST CHAR* skin)
{
	CHAR sz_filename[FILENAME_MAX]{};
		system("chcp 1251");
	for (int i = 0; i < 18; i++)
	{
		HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + i);
		sprintf(sz_filename, "ButtonBMP\\%s\\%s.bmp", skin, g_BUTTON_FILENAME[i]);

		HBITMAP hBitmap = (HBITMAP)LoadImage
		(
			GetModuleHandle(NULL),
			sz_filename,
			//"ButtonBMP\\square_blue\\button_0.bmp",
			IMAGE_BITMAP,
			i + IDC_BUTTON_0 == IDC_BUTTON_0 ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			i + IDC_BUTTON_0 == IDC_BUTTON_EQUAL ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
		);

		/*HBITMAP bmpButton = (HBITMAP)LoadImage;
		(
			GetModuleHandle(NULL),
			sz_filename,
			IMAGE_BITMAP,
			i + IDC_BUTTON_0 == IDC_BUTTON_0 ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			i + IDC_BUTTON_0 == IDC_BUTTON_EQUAL ? g_i_BUTTON_DOUBLE_SIZE : g_i_BUTTON_SIZE,
			LR_LOADFROMFILE
			);*/
		//setlocale(LC_ALL, "");
		std::cout << FormatLastError() << std::endl;
		std::cout << sz_filename << std::endl;
		SendMessage(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}
}

