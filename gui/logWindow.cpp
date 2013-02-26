#include "logWindow.h"
#include <Windows.h>
#include <tchar.h>
#include <windowsx.h>

#define ID_EDITCHILD 100

static HWND hwndEdit;

int initLogWindow(HWND parent, HINSTANCE hInstance, int nCmdShow) {

	TCHAR szWindowClass[] = _T("win32app");
	TCHAR szTitle[] = _T("Log");

	HWND hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        parent,
        NULL,
        hInstance,
        NULL
    );

	if (!hWnd)
    {

        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	return 0;
}

HWND createLogTextArea(HWND parentWindow, HINSTANCE hInstance) {

	hwndEdit = CreateWindowEx(0, _T("EDIT"),   // predefined class 
                          NULL,         // no window title 
                          WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
                          ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
                          0, 0, 0, 0,   // set size in WM_SIZE message 
                          parentWindow,         // parent window 
                          (HMENU) ID_EDITCHILD,   // edit control ID 
						  //NULL,
                          (HINSTANCE) GetWindowLong(parentWindow, GWL_HINSTANCE), 
						  //hInstance,
                          NULL);        // pointer not needed 
	return hwndEdit;
}

void appendLogText(LPCTSTR newText)
{
	/*DWORD l,r;
	SendMessage(hwndEdit, EM_GETSEL,(WPARAM)&l,(LPARAM)&r);
	SendMessage(hwndEdit, EM_SETSEL, -1, -1);
	SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM)newText);
	SendMessage(hwndEdit, EM_SETSEL,l,r);*/

	DWORD sel = Edit_GetSel(hwndEdit);
	Edit_SetSel(hwndEdit, -1, -1);
	Edit_ReplaceSel(hwndEdit, newText);
	Edit_SetSel(hwndEdit, LOWORD(sel), HIWORD(sel));
}
