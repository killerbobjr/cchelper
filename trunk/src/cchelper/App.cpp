#include "Webcontrol.h"
#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "CGChessWnd.h"
#include "ChessEngine.h"
#include "AppEnv.h"
#include "ChessBoard.h"
#include "resource.h"
#include "mousehook.h"


using namespace base;


// GLOBAL VALUES
//___________________________________________________________________________

CChessEngine	* g_pChessEngine = NULL;
CChessBoard		* g_pChessBoard = NULL;

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

	// add qq new chess window
	IGameWindow * pGameWnd = new CQQNewChessWnd();

	if(!pGameWnd->LoadHashValue(_T("hv_qq.ini")))
	{
		return FALSE;
	}

	g_pChessBoard->RegisterGameWindow(pGameWnd);


	// add china game chess window
	pGameWnd = new CGChessWnd();

	if(!pGameWnd->LoadHashValue(_T("hv_cg_22.ini")))
	{
		return FALSE;
	}

	g_pChessBoard->RegisterGameWindow(pGameWnd);


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


	return TRUE;
}

BOOL AppLoop()
{
	IGameWindow * pgw;

	pgw = g_pChessBoard->GetGameWindow();

	if( !pgw )
	{
		pgw = g_pChessBoard->SearchGameWindow();
		if( pgw )
		{
			g_pChessBoard->SetGameWindow(pgw);
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

	if ( g_pChessEngine) delete g_pChessEngine;

	UnEmbedBrowserObject(g_hWndMain);

	return TRUE;
}


