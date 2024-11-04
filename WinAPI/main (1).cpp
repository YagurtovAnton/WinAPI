#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_LOGIN_INVITATION[] = "Введите имя пользователя";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	/*
	------------------------------------
	hInstance - экземпляр загруженного *.exe-файла нашей программы.
	hPrevInst - предыдущий запущенный экземпляр. Этот параметр уже не используется,
				и оставлен только для совместимости.
	lpCmdLine - командная строка, с которой запустилась программа.
				Содержит имя имполняемого файла, и параметры запуска,
				например, имя отрываемого файла, URL Интернет-ресурса и т.д.
				Drag'n Drop так же работает через командную строку.
	LPSTR - Long Pointer to String
	nCmdShow  - Состояние окна при запуске, развернуто на весь экран, свернуто в окно,
				свернуто на панель задач.....
	------------------------------------
	*/
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	----------------------------
	hwnd (Handler to Windows) - дескриптор окна, это число, через которое к нему можно обратиться.
	----------------------------
	uMsg (Unsigned Message)   - сообщение, которое отправляется в окно.
	----------------------------
	wParam, lParam - параметры сообщения, зависят от сообщения, которое отправляется в окно.
	DWORD (DOUBLE WORD) - двойное слово (4 Байта = 32 бита);
	----------------------------
	*/
	switch (uMsg)
	{
	
		case IDOK:		MessageBox(hwnd, "Была нажата кнопка OK!", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL:	EndDialog(hwnd, 0); break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;
}