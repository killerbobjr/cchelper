// cchess_tool.cpp : Defines the entry point for the application.
//
#include "common.h"
#include "Resource.h"
#include "QQNewChessWnd.h"
#include "fastdib.h"

#define MAX_LOADSTRING	256

/*

[D2FC85BF][17B56D39][C4F9C567][826AA68B][18233962][826AA68B][C4F9C567][17B56D39][D2FC85BF]
[831D7709][C793D8C6][A2E9D8F9][DC8353E0][127F2FCE][6146BDDF][309BB9FD][5D5029F4][823C05F6]
[14FBA89F][402355D8][D9E6CBDA][603A80C0][8A7A5CEC][C77D1B78][1379B3CF][402355D8][A33A6680]
[03A331A5][BBAC72F0][03A331A5][E9C87800][03A331A5][5FF9813D][03A331A5][82BC3990][03A331A5]
[3E5A3D0A][9BBE1A86][73D648F4][4FEDA8C2][78DCCF13][83AE2320][DCC6ECF6][77D0A954][DC67BB41]
[1704FD28][5334CEB0][BCECC505][6DEA08CA][F533B8FE][DFC66E2D][7B305646][6F818D66][5B2A348B]
[9291F591][E27C4114][9291F591][EEE5BB96][9291F591][EDA8C367][9291F591][82D8A68C][9291F591]
[541EAF84][737E5B02][1E0DA9D8][38632858][881CC997][25186C60][897868CC][737E5B02][C94C8A61]
[4437F4F1][F4F08F5D][360E7182][739B5ADF][CD5B6E39][0BF693C1][98CF2FFC][8D454D36][0D554F46]
[56FC44E8][5699E66C][7AA76288][2BC9BFCC][651E8CBB][2BC9BFCC][7AA76288][5699E66C][56FC44E8]

[J]-[M]-[X]-[S]-[K]-[S]-[X]-[M]-[J]
 |   |   |   | + | + |   |   |   |
 +---+---+---+---+---+---+---+---+
 |   |   |   | + | + |   |   |   |
 +--[P]--+---+---+---+---+--[P]--+
 |   |   |   |   |   |   |   |   |
[B]--+--[B]--+--[B]--+--[B]--+--[B]
 |   |   |   |   |   |   |   |   |
 +-------------------------------+
 |  Chu  River        Han Border |
 +-------------------------------+
 |   |   |   |   |   |   |   |   |
<b>--+--<b>--+--<b>--+--<b>--+--<b>
 |   |   |   |   |   |   |   |   |
 +--<p>--+---+---+---+---+--<p>--+
 |   |   |   | + | + |   |   |   |
 +---+---+---+---+---+---+---+---+
 |   |   |   | + | + |   |   |   |
<j>-<m>-<x>-<s>-<k>-<s>-<x>-<m>-<j>

*/
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int g_x = 0, g_y = 0;

void GetWindowInformation();

COLORREF * g_pBoardRectPic = NULL;

CQQNewChessWnd * g_pQcnWnd = NULL;

POINT	g_ptMouse;
int g_intAppRunning;
HWND g_hWnd;
CFastDIB * g_pMainSurface = NULL;

struct WindowInformation
{
	HWND hwnd;
	TCHAR szTitle[MAX_LOADSTRING];
	TCHAR szWindowClass[MAX_LOADSTRING];
	WINDOWINFO wi;
} g_WinInfo;


void DrawBackground()
{

	HRESULT hr ;

	CFastDIB * fdib;

	HDC hdc = GetDC(g_hWnd);

	if ( g_pMainSurface )
	{
		fdib = new CFastDIB();

		hr = fdib->LoadFromFile(_T("../../media/CBOARD.bmp"));
		_CHECK(hr);
		
		if ( hr == S_OK )
		{
			_CHECK(fdib->Draw( g_pMainSurface ));
		}

		_CHECK(g_pMainSurface->FastBlt(hdc));

		if ( fdib ) 
			delete fdib;
	}

}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CCHELPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CCHELPER));
	
	CFastDIB::Initialize(1024,768);
	g_pMainSurface = new CFastDIB();

	_CHECK(g_pMainSurface->CreateDIB( 800, 600, FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF ));
	

	g_pQcnWnd = new CQQNewChessWnd();

	SetTimer(g_hWnd,             // handle to main window 
		1,            // timer identifier 
		500,                 // 10-second interval 
		(TIMERPROC) NULL);     // no timer callback 

	// Main message loop:
	g_intAppRunning = TRUE;
	while(g_intAppRunning)
	{
		if(PeekMessage(&msg, g_hWnd, 0, 0, PM_REMOVE))
		{
			if(!IsDialogMessage(g_hWnd,&msg))
			{
				DispatchMessage(&msg);
			}
		} else 
		{
			//GetWindowInformation();
			if( !g_pQcnWnd->GetHwnd() )
			{
				g_pQcnWnd->FindQQNewChessWindow();
			}

			DrawBackground();
			Sleep(20);
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

	HWND hwndEdit = GetDlgItem(g_hWnd,ID_EDITCHILD);

	_stprintf_s(buf, _T("「WindowClass」=%s 「CtrlID」=0x%p 「Location」=%d,%d"), g_WinInfo.szWindowClass, id, 
		g_WinInfo.wi.rcWindow.right - g_WinInfo.wi.rcWindow.left  , 
		g_WinInfo.wi.rcWindow.bottom - g_WinInfo.wi.rcWindow.top);

	// Add text to the window. 
	//SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM) buf);

	InvalidateRect(g_hWnd,NULL,FALSE);
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
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CCHELPER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Store instance handle in our global variable

	g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	//HWND hwndEdit = CreateWindow(_T("EDIT"),      // predefined class 
	//						(LPCWSTR)NULL,        // no window title 
	//						WS_CHILD | WS_VISIBLE | WS_VSCROLL | 
	//						ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL, 
	//						0, 30, 400, 100,  // set size in WM_SIZE message 
	//						g_hWnd,        // parent window 
	//						(HMENU) ID_EDITCHILD,   // edit control ID 
	//						(HINSTANCE) hInst, 
	//						NULL);       // pointer not needed 


	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
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

void DumpBoradHash()
{
	TCHAR buf[1024];

	DWORD k;
	int i;
	int j;

	for( j =0; j < 10; j++ )
	{
		OutputDebugString( _T("\n") );
		for( i = 0; i < 9; i ++)
		{
			k = g_pQcnWnd->GetHash(i,j);
			_stprintf_s(buf, _T("[%4p]"), k);
			OutputDebugString( buf );
		}
	}
}

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	WCHAR buf[256];
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int vk;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), g_hWnd, About);			
			break;
		case IDM_EXIT:
			DestroyWindow(g_hWnd);
			break;
		default:
			return DefWindowProc(g_hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(g_hWnd, &ps);

		if ( g_pQcnWnd && g_pQcnWnd->GetHwnd() )
		{
			_stprintf_s(buf, _T("Mouse(%4d,%4d) , (ox, oy)=(%4d,%4d) , (dx,dy) =(%4d,%4d)"), g_ptMouse.x, g_ptMouse.y,
				g_pQcnWnd->ox, g_pQcnWnd->oy, g_pQcnWnd->dx, g_pQcnWnd->dy );				
			TextOut(hdc, 0, 0,(LPCWSTR) buf, wcslen(buf));
			DrawBoardRectPic(hdc);
		}

		
		EndPaint(g_hWnd, &ps);
		break;
	case WM_KEYUP:
		if ( g_pBoardRectPic )
		{
			delete g_pBoardRectPic ;
			g_pBoardRectPic = NULL;
		}

		if ( g_pQcnWnd &&  g_pQcnWnd->GetHwnd() ) 
		{
			vk = (int) wParam;

			switch( vk )
			{
			case VK_F2:
				g_pQcnWnd->dx --;
				break;
			case VK_F3:
				g_pQcnWnd->dx ++;
				break;

			case VK_F4:
				g_pQcnWnd->dy --;
				break;
			case VK_F5:
				g_pQcnWnd->dy ++;
				break;

			case  VK_LEFT:
				g_pQcnWnd->ox --;
				break;
			case VK_RIGHT:
				g_pQcnWnd->ox ++;
				break;
			case VK_UP:
				g_pQcnWnd->oy --;
				break;
			case VK_DOWN:
				g_pQcnWnd->oy ++;
				break;
			case VK_SPACE:
				g_x =(int)( (double)rand() / (RAND_MAX + 1) * 9 );
				g_y =(int)( (double)rand() / (RAND_MAX + 1) * 10 );
				break;
			case VK_RETURN:
				DumpBoradHash();
				break;
			}
			g_pBoardRectPic = g_pQcnWnd->GetRectData( g_x, g_y) ;
			InvalidateRect(g_hWnd,NULL,FALSE);
		}
		break;
	case WM_TIMER:
		switch( wParam )
		{
		case 1:
			if( g_pQcnWnd && g_pQcnWnd->GetHwnd() )
			{
				if ( g_pBoardRectPic )
				{
					delete g_pBoardRectPic ;
					g_pBoardRectPic = NULL;
				}
				
				g_x =(int)( (double)rand() / (RAND_MAX + 1) * 9 );
				g_y =(int)( (double)rand() / (RAND_MAX + 1) * 10 );
				g_pBoardRectPic = g_pQcnWnd->GetRectData( g_x, g_y) ;
				InvalidateRect(g_hWnd,NULL,FALSE);
			}
			return 0;
		}
		break;
	case WM_DESTROY:
		g_intAppRunning = FALSE;

		if ( g_pMainSurface ) 
		{
			delete g_pMainSurface;
			g_pMainSurface = NULL;
		}

		if ( g_pQcnWnd ) delete g_pQcnWnd;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(g_hWnd, message, wParam, lParam);
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
