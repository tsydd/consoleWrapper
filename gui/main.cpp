// GT_HelloWorldWin32.cpp
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include "console.h"
#include "logWindow.h"
#include "util.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <Richedit.h>

#define BUFSIZE 4096 

// Global variables

CConsole console;
HANDLE g_hReaderThreadHandle;

// The main window class name.
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND CreateRichEdit(HWND hwndOwner, int x, int y, int width, int height, HINSTANCE hinst);

HWND mainWindow;

void startConsole(void);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	//Console console();
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Win32 Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance; // Store instance handle in our global variable

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application dows not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = mainWindow = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        500, 100,
        NULL,
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
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

	initLogWindow(hWnd, hInstance, nCmdShow);



    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

	static HWND hwndEdit;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
	case WM_COMMAND:
		//appendLogText(_T("command"));
		break;
	case WM_CREATE:
		{
			LPCREATESTRUCT createStruct = (LPCREATESTRUCT) lParam;
			if (!createStruct->hwndParent) {
				CreateRichEdit(hWnd, 0, 0, 800, 600, hInst);		
			} else {
				hwndEdit = createLogTextArea(hWnd, hInst);
				startConsole();
			}

			break;
		}
    case WM_DESTROY:		
		if (hWnd == mainWindow) {
			PostQuitMessage(0);
			TerminateThread(g_hReaderThreadHandle, 0);
		}
        break;
	case WM_SIZE:
        // Make the edit control the size of the window's client area. 
		//appendLogText(_T("WM_SIZE\r\n"));
		if (hWnd != mainWindow) {
			MoveWindow(hwndEdit, 
                   0, 0,                  // starting x- and y-coordinates 
                   LOWORD(lParam),        // width of client area 
                   HIWORD(lParam),        // height of client area 
                   TRUE);                 // repaint window 
		}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

HWND CreateRichEdit(HWND hwndOwner,        // Dialog box handle.
                    int x, int y,          // Location.
                    int width, int height, // Dimensions.
                    HINSTANCE hinst)       // Application or DLL instance.
{
    LoadLibrary(TEXT("Msftedit.dll"));
    
    HWND hwndEdit= CreateWindowEx(0, MSFTEDIT_CLASS, TEXT("Type here"),
        ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP, 
        x, y, width, height, 
        hwndOwner, NULL, hinst, NULL);
        
    return hwndEdit;
}

void startConsole()
{	
	console.Start();
	DWORD returnCode;
	g_hReaderThreadHandle = CreateThread( 
		NULL,                   // default security attributes
		0,                      // use default stack size  
		consoleReaderThreadFunction,       // thread function name
		NULL,          // argument to thread function 
		0,                      // use default creation flags 
		&returnCode);   // returns the thread identifier 

	if (g_hReaderThreadHandle == NULL) 
	{
		ErrorExit(TEXT("CreateThread"));
		ExitProcess(3);
	}
}

DWORD WINAPI consoleReaderThreadFunction(LPVOID params)
{
	DWORD dwRead, dwWritten; 
	CHAR chBuf[BUFSIZE];
	TCHAR lpctBuf[BUFSIZE];
	
	BOOL bSuccess = FALSE;
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	for (;;) 
	{ 
		bSuccess = ReadFile( console.m_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL);
		if( ! bSuccess || dwRead == 0 ) break; 
		
#ifdef UNICODE
		size_t convertedSize;
		mbstowcs_s(&convertedSize, lpctBuf, chBuf, dwRead);
#else
		lpctBuf = chBuf;
#endif
		appendLogText(lpctBuf);
	} 

	return 0;
}