
#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"
#include "AppEnv.h"
#include "ChessBoard.h"
#include "resource.h"
#include <mmsystem.h>

using namespace base;


// GLOBAL VALUES
//___________________________________________________________________________
IGameWindow		* g_pGameWnd = NULL;
CQQNewChessWnd	* g_pQncWnd = NULL;
CChessEngine	* g_pChessEngine = NULL;
CChessBoard		* g_pChessBoard = NULL;
BOOL			g_bAlarmFlage = FALSE;

BOOL InitApp()
{	
	if(!AppEnv::LoadEnv(_T("./cchelper.ini")))
		return FALSE;

	if(AppEnv::nThinkTime == 5)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC5, MF_CHECKED);
	else if(AppEnv::nThinkTime == 10)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC10, MF_CHECKED);
	else if(AppEnv::nThinkTime == 30)	CheckMenuItem(GetMenu(g_hWndMain), IDM_SEC30, MF_CHECKED);

	if( !CChessBoard::LoadMedia() )
	{
		return FALSE;
	}

	g_pQncWnd = new CQQNewChessWnd();

	if(!g_pQncWnd->LoadHashValue(_T("hv_qq.ini")))
	{
		return FALSE;
	}


	g_pChessBoard = new CChessBoard();

	g_pChessEngine = new CChessEngine();

	g_pChessBoard->SetChessEngine( g_pChessEngine );


#ifdef ENGINE_CCE
	g_pChessEngine->InitEngine("cce.exe");
#else
	if(g_pChessEngine->InitEngine(AppEnv::szEngine))
	{
		base::Log(0,"Load engine success");
	}
	else 
	{
		base::Log(0,"Load engine failed");
		return FALSE;
	}
#endif
	assert(g_pChessEngine->IsLoaded() );
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
		g_pChessBoard->DrawBoard( NULL );
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

	CChessBoard::ReleaseMedia();

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
