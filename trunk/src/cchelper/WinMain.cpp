// cchess_tool.cpp : Defines the entry point for the application.
//
#include "common.h"
#include "Resource.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"
#include "fastdib.h"
#include "app.h"
#include "appenv.h"
#include <mmsystem.h>

// STRUCTS
//___________________________________________________________________________
struct WindowInformation
{
	HWND hwnd;
	TCHAR szTitle[MAX_LOADSTRING];
	TCHAR szWindowClass[MAX_LOADSTRING];
	WINDOWINFO wi;
} g_WinInfo;


// Forward declarations of functions included in this code module:
//___________________________________________________________________________
ATOM				MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
VOID GetWindowInformation();


// GLOBAL VARIABLES DECLARATIONS
//___________________________________________________________________________
POINT		g_ptMouse;
int			g_intAppRunning;
BOOL		g_bActive;
HWND		g_hWndMain;
BOOL		g_bInitialized	= FALSE;
CFastDIB	*g_pMainSurface = NULL;


HINSTANCE	hInst;								// current instance
TCHAR		szTitle[MAX_LOADSTRING];			// The title bar text
TCHAR		szWindowClass[MAX_LOADSTRING];		// the main window class name

int g_x = 0, g_y = 0;				// for test
COLORREF * g_pBoardRectPic = NULL;	// for test


//FUNCTIONS
//___________________________________________________________________________


void AlreadyRun(void)
{
	HWND	FirsthWnd, FirstChildhWnd;

	if((FirsthWnd = FindWindowEx(NULL, NULL, szWindowClass, NULL )) != NULL)
	{
		FirstChildhWnd = GetLastActivePopup(FirsthWnd);
		SetForegroundWindow(FirsthWnd);
	
		if(FirsthWnd != FirstChildhWnd)
		{
			SetForegroundWindow(FirstChildhWnd);
		}
		
		ShowWindow(FirsthWnd, SW_SHOWNORMAL);
	}
}


BOOL doInit(HINSTANCE hInstance, int nCmdShow)
{
	HACCEL hAccelTable;

	MyRegisterClass(hInstance);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CCHELPER));

    //  create a window
    g_hWndMain = CreateWindowEx(WS_EX_TOPMOST,
                          szWindowClass,
                          szTitle,
                          WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
                          0,
                          0,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    if(!g_hWndMain) return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CCHELPER));

	SetTimer( g_hWndMain, APPUPDATE_TIMER, APPUPDATE_TIMER_ELAPSE, 0 );

	if(!InitApp())
		return FALSE;

	g_bInitialized = TRUE;

    ShowWindow(g_hWndMain, nCmdShow);
    UpdateWindow(g_hWndMain);

    return TRUE;
} 

BOOL doUpdate()
{
	HDC hdc = GetDC(g_hWndMain);

	if ( g_pMainSurface && hdc )
	{
		g_pMainSurface->FastBlt(hdc);

		ReleaseDC(g_hWndMain, hdc);
	}
	return TRUE;
}

BOOL doRelease()
{
	g_intAppRunning = FALSE;

	if ( g_pMainSurface ) 
	{
		delete g_pMainSurface;
		g_pMainSurface = NULL;
	}
	return TRUE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CCHELPER, szWindowClass, MAX_LOADSTRING);

	HANDLE	hMutex;

	hMutex = CreateMutex(NULL, TRUE, szWindowClass);
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		AlreadyRun();
		return 0;
	}

	if( !doInit(hInstance, nCmdShow) )
		return -1;

    for (;;)
    {
        if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
        {
            if(!GetMessage(&msg,NULL,0,0))
            {
                return msg.wParam;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }else
		{
			//if( !AppLoop() )
			//	DestroyWindow(g_hWndMain);
			//else
			WaitMessage();
		}

    }


	return (int) msg.wParam;
}

void GetWindowInformation()
{

	TCHAR buf[256];

	GetCursorPos (&g_ptMouse);

	memset(&g_WinInfo, 0, sizeof(g_WinInfo));

	g_WinInfo.hwnd  = WindowFromPoint(g_ptMouse);

	RealGetWindowClass(g_WinInfo.hwnd , g_WinInfo.szWindowClass, sizeof(g_WinInfo.szWindowClass));
	GetWindowInfo(g_WinInfo.hwnd , &g_WinInfo.wi );

	int id = GetDlgCtrlID(g_WinInfo.hwnd );

	HWND hwndEdit = GetDlgItem(g_hWndMain,ID_EDITCHILD);

	_stprintf_s(buf, _T("「WindowClass」=%s 「CtrlID」=0x%p 「Location」=%d,%d"), g_WinInfo.szWindowClass, id, 
		g_WinInfo.wi.rcWindow.right - g_WinInfo.wi.rcWindow.left  , 
		g_WinInfo.wi.rcWindow.bottom - g_WinInfo.wi.rcWindow.top);

	// Add text to the window. 
	//SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) buf);

	InvalidateRect(g_hWndMain,NULL,FALSE);
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CCHELPER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CCHELPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

void 	DrawBoardRectPic(HDC hdc)
{
	if ( g_pBoardRectPic )
	{
		for( int i =0; i < 58; i ++ )
			for ( int j = 0; j < 58 ; j ++ )
			{
				SetPixel(hdc, 100 + i, 200 + j , g_pBoardRectPic[j * 58 + i]);
			}
	}
}

LRESULT CALLBACK WndProc(HWND g_hWndMain, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL bAlarm = FALSE;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_ACTIVATEAPP:
		g_bActive = wParam;
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			//test();
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), g_hWndMain, About);
			break;
		case IDM_EXIT:
			DestroyWindow(g_hWndMain);
			break;
		case IDM_SEC5:
			AppEnv::nThinkTime = 5;			
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC5, MF_CHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_UNCHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_UNCHECKED);
			break;
		case IDM_SEC10:
			AppEnv::nThinkTime = 10;
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC5, MF_UNCHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_CHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_UNCHECKED);
			break;
		case IDM_SEC30:
			AppEnv::nThinkTime = 30;
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC5, MF_UNCHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_UNCHECKED);
			CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_CHECKED);
			break;
		case IDM_AUTOPLAY:
			AppEnv::bAutoPlay = !AppEnv::bAutoPlay ;
			if(AppEnv::bAutoPlay )
				CheckMenuItem(GetMenu(g_hWndMain), IDM_AUTOPLAY, MF_CHECKED);
			else
				CheckMenuItem(GetMenu(g_hWndMain), IDM_AUTOPLAY, MF_UNCHECKED);			
		default:
			return DefWindowProc(g_hWndMain, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(g_hWndMain, &ps);

		if ( g_pMainSurface  )
		{
			g_pMainSurface->FastBlt(hdc);
		}

		EndPaint(g_hWndMain, &ps);
		break;
	case WM_LBUTTONUP:
		break;
	//case WM_KEYUP:
	//		vk = (int) wParam;

	//		switch( vk )
	//		{
	//		case VK_RETURN:
	//			if ( g_pChessEngine)
	//			{
	//				if ( g_pChessEngine->GetState() == CChessEngine::BusyWait )
	//					g_pChessEngine->SendCommand("stop");
	//			}
	//			//DumpBoradHash();
	//			break;
	//		}
		break;
	case WM_TIMER:
		switch( wParam )
		{
		case APPUPDATE_TIMER:
			if (!AppLoop()) {
			    DestroyWindow(g_hWndMain);
			}
			doUpdate();

			return 0;

		}
		break;
	case WM_DESTROY:
		if(g_bInitialized)
		{
			ExitApp();
		}

		doRelease();

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(g_hWndMain, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

