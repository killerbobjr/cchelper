#include "Webcontrol.h"
#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"
#include "AppEnv.h"
#include "ChessBoard.h"
#include "resource.h"
#include "mousehook.h"

#include <mmsystem.h>

using namespace base;


// GLOBAL VALUES
//___________________________________________________________________________
IGameWindow		* g_pGameWnd = NULL;
CQQNewChessWnd	* g_pQncWnd = NULL;
CChessEngine	* g_pChessEngine = NULL;
CChessBoard		* g_pChessBoard = NULL;
BOOL			g_bAlarmFlage = FALSE;
HHOOK			g_hHookPlay = NULL;
HHOOK			g_hHookGetMessage = NULL;

// GLOBAL FUNCTION
//_____________________________________________________________________________
void SetWindowSize(DWORD dwWidth,DWORD dwHeight)
{
	RECT  rc;
	int nAdvertisementHeight = 40;

	// Ajust window size
	SetRect( &rc, 0, 0, dwWidth, dwHeight + nAdvertisementHeight );

	AdjustWindowRectEx( &rc, GetWindowStyle(g_hWndMain), GetMenu(g_hWndMain) != NULL,
						GetWindowExStyle(g_hWndMain) );

	MoveWindow(g_hWndMain,
		rc.left+(GetSystemMetrics(SM_CXSCREEN)-dwWidth)/2,
		rc.top+(GetSystemMetrics(SM_CYSCREEN)-dwHeight)/2,
		rc.right-rc.left,
		rc.bottom-rc.top,
		FALSE );

	CFastDIB::Initialize(dwWidth, dwHeight);

	g_pMainSurface = new CFastDIB();

	g_pMainSurface->CreateDIB( dwWidth, dwHeight, FDIBTYPE_RGBA );

	EmbedBrowserObject(g_hWndMain);

	
	ResizeBrowser(g_hWndMain, 0, dwHeight, dwWidth, nAdvertisementHeight);

	DisplayHTMLPage(g_hWndMain,_T("http://images.sohu.com/bill/s2009/jiedong/market/nba/450105-2.swf"));

}

LRESULT CALLBACK GetMsgProc(int code,
    WPARAM wParam,
    LPARAM lParam
)
{
	if(code < -1 )
	{
		return CallNextHookEx(g_hHookPlay, code, wParam, lParam);
	}

	MSG * pMsg = (MSG*)lParam;
	switch(pMsg->message)
	{
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCRBUTTONDBLCLK:
		if(pMsg->hwnd != g_hWndMain)
		{
			OutputDebugString("Got a mouse message\n");
			pMsg->hwnd = g_hWndMain;
		}
	}
	return CallNextHookEx(g_hHookPlay, code, wParam, lParam);
}

LRESULT CALLBACK JournalPlaybackProc(int code,
    WPARAM wParam,
    LPARAM lParam
)
{
	switch(code)
	{
	case HC_GETNEXT:
		break;
	case HC_NOREMOVE:
		break;
	case HC_SKIP:
		break;
	case HC_SYSMODALOFF:
		break;
	case HC_SYSMODALON:
		break;
	}
	return CallNextHookEx(g_hHookPlay, code, wParam, lParam);
}

BOOL InitApp()
{

	if(!AppEnv::LoadEnv(_T("./cchelper.ini")))
		return FALSE;

	if(AppEnv::nThinkTime == 5)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC5, MF_CHECKED);
	else if(AppEnv::nThinkTime == 10)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_CHECKED);
	else if(AppEnv::nThinkTime == 30)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_CHECKED);



	g_pChessBoard = new CChessBoard();

	g_pChessEngine = new CChessEngine();

	g_pChessBoard->SetChessEngine( g_pChessEngine );

	if( !g_pChessBoard->LoadMedia() )
	{
		return FALSE;
	}

	SIZE sizeBoard;

	g_pChessBoard->GetBoardSize(&sizeBoard);

	SetWindowSize(sizeBoard.cx ,sizeBoard.cy);

	g_pQncWnd = new CQQNewChessWnd();

	if(!g_pQncWnd->LoadHashValue(_T("hv_qq.ini")))
	{
		return FALSE;
	}


	if(g_pChessEngine->InitEngine(AppEnv::szEngine))
	{
		base::Log(0,"Load engine success");
	}
	else 
	{
		base::Log(0,"Load engine failed");
		return FALSE;
	}

	assert(g_pChessEngine->IsLoaded() );

	// hook mouse
	//CMouseHook::StartHook( g_hWndMain );
	
	//g_hHookPlay = SetWindowsHookEx(WH_JOURNALPLAYBACK,
	//	(HOOKPROC) JournalPlaybackProc, GetModuleHandle(NULL),0);


	//g_hHookGetMessage= SetWindowsHookEx(WH_GETMESSAGE ,
	//	(HOOKPROC) GetMsgProc, GetModuleHandle(NULL),0);

	return TRUE;
}

BOOL AppLoop()
{
	IGameWindow * pgw;

	pgw = g_pChessBoard->GetGameWindow();

	HWND hwnd = g_pQncWnd->GetHandle();
	if( !pgw )
	{
		if(g_pQncWnd->FindQQNewChessWindow())
		{
			g_pChessBoard->SetGameWindow(g_pQncWnd);
		}
		g_pChessBoard->DrawBoard( "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1" );
	} 
	else
	{
		g_pChessBoard->Update();
	}

	return TRUE;
}

BOOL ExitApp()
{
	if( g_pChessBoard ) delete g_pChessBoard;

	if ( g_pQncWnd ) delete g_pQncWnd;

	if ( g_pChessEngine) delete g_pChessEngine;

	UnEmbedBrowserObject(g_hWndMain);

	//CMouseHook::StopHook();
	//if(g_hHookPlay)
	//	UnhookWindowsHookEx(g_hHookPlay);
	//if(g_hHookGetMessage)
	//{
	//	assert(UnhookWindowsHookEx(g_hHookGetMessage));
	//}
	return TRUE;
}


void SetAlarm()
{
	if( !g_bAlarmFlage )
	{
		PlaySound( AppEnv::GetMediaPath(_T("alarm.wav")), NULL,  SND_FILENAME|SND_LOOP|SND_ASYNC);
		g_bAlarmFlage = TRUE;
	}
}

void KillAlarm()
{
	if( g_bAlarmFlage )
	{
		PlaySound( NULL, 0, SND_ASYNC);
		g_bAlarmFlage = FALSE;
	}
}
