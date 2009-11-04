#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"
#include "AppEnv.h"
#include "resource.h"

using namespace base;


// GLOBAL VALUES
//___________________________________________________________________________

CQQNewChessWnd	* g_pQcnWnd = NULL;
CChessEngine	* g_pChessEngine = NULL;
CChessBoard		* g_pBoard = NULL;


BOOL InitApp()
{
	if(!AppEnv::LoadEnv(_T("./cchelper.ini")))
		return FALSE;

	if(AppEnv::nThinkTime == 10)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_CHECKED);
	else if(AppEnv::nThinkTime == 30)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_CHECKED);
	else if(AppEnv::nThinkTime == 90)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC90, MF_CHECKED);

	if( !CChessBoard::LoadMedia() )
	{
		return FALSE;
	}

	g_pQcnWnd = new CQQNewChessWnd();

	g_pBoard = new CChessBoard();

	g_pChessEngine = new CChessEngine();

#ifdef ENGINE_CCE
	g_pChessEngine->InitEngine("cce.exe");
#else
	g_pChessEngine->InitEngine(AppEnv::szEngine);
#endif
	assert(g_pChessEngine->IsLoaded() );
	return TRUE;
}

BOOL AppLoop()
{
	static char fenCopy[256];

	char szCmd[1024];
	static int lastturn=0;

	HWND hwnd = g_pQcnWnd->GetHandle();
	if( !hwnd )
	{
		g_pQcnWnd->FindQQNewChessWindow();
		g_pBoard->DrawBoard( NULL );
	} 
	else
	{
		GAMEINFO gi;
		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd,&wp);

		if (wp.showCmd == SW_SHOWNORMAL)
		{
			memset( &gi, 0, sizeof(gi));
			if( g_pQcnWnd->ReadWindow(&gi) )
			{
				g_pBoard->DrawBoard( &gi );
				if(strcmp(fenCopy, gi.szFen ) != 0 && lastturn != gi.Turn )
				{
					strcpy_s(fenCopy, 256, gi.szFen);
					lastturn = gi.Turn ;

					if( gi.PlayerColor == gi.Turn  )
					{
						if( g_pChessEngine->IsLoaded() )
						{
							if( g_pChessEngine->GetState() == CChessEngine::BusyWait)
							{
								g_pChessEngine->Stop();
							}
							sprintf(szCmd, "position fen %s", gi.szFen );
							g_pChessEngine->SendCommand(szCmd);
							sprintf(szCmd, "go time %d", AppEnv::nThinkTime );
							g_pChessEngine->SendCommand(szCmd);
						}
					}
				}else 
				{
					if( gi.Turn == gi.PlayerColor && g_pChessEngine )
					{
						CChessEngine::PieceMove * mv;
						mv = g_pChessEngine->GetBestMove();
						if ( mv )
						{
							if ( gi.PlayerColor == TURN_WHITE )
								g_pBoard->ShowBestMove(mv->fx, mv->fy, mv->tx, mv->ty);
							else if (gi.PlayerColor == TURN_BLACK)
								g_pBoard->ShowBestMove(mv->fx, 9 - mv->fy, mv->tx, 9 - mv->ty);

						}
					}
				}
			} 

			if( g_pChessEngine)
			{
				g_pChessEngine->UpdateState();
			}
		}
	}
	//if(1) // test ocde
	//{
	//	GAMEINFO tgi;
	//	tgi.PlayerColor = TURN_WHITE;
	//	strcpy(tgi.szFen, "jmxsksxmj/9/1p5p1/b1b1b1b1b/9/9/B1B1B1B1B/1P5P1/9/JMXSKSXMJ w");
	//	tgi.Turn = TURN_WHITE;

	//	g_pBoard->DrawBoard(&tgi);
	//	g_pBoard->ShowBestMove(0,0,2,2);
	//}
	//
	//InvalidateRect(g_hWndMain,NULL,FALSE);
	return TRUE;
}

BOOL ExitApp()
{
	if( g_pBoard ) delete g_pBoard;

	if ( g_pQcnWnd ) delete g_pQcnWnd;

	if ( g_pChessEngine) delete g_pChessEngine;

	CChessBoard::ReleaseMedia();

	return TRUE;
}