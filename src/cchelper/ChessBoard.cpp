#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "ChessEngine.h"
#include "ChessBoard.h"
#include "AppEnv.h"
#include "resource.h"

using namespace base;

// CChessBoard
//___________________________________________________________________________
CFastDIB * CChessBoard::m_pBoardDIB = NULL;
CFastDIB * CChessBoard::m_pMoveRectDIB = NULL;
CFastDIB * CChessBoard::m_pBackgroundDIB = NULL;

//PieceStruct CChessBoard::m_tPieceStructs[PIECE_NUM] = {
//	{ 'J', PIECEHV_RJ, MEDIAFILE(_T("CRJ.BMP")), 0 },
//	{ 'M', PIECEHV_RM, MEDIAFILE(_T("CRM.BMP")), 0 },
//	{ 'X', PIECEHV_RX, MEDIAFILE(_T("CRX.BMP")), 0 },
//	{ 'S', PIECEHV_RS, MEDIAFILE(_T("CRS.BMP")), 0 },
//	{ 'K', PIECEHV_RK, MEDIAFILE(_T("CRK.BMP")), 0 },
//	{ 'P', PIECEHV_RP, MEDIAFILE(_T("CRP.BMP")), 0 },
//	{ 'B', PIECEHV_RB, MEDIAFILE(_T("CRB.BMP")), 0 },
//	{ 'j', PIECEHV_BJ, MEDIAFILE(_T("CBJ.BMP")), 0 },
//	{ 'm', PIECEHV_BM, MEDIAFILE(_T("CBM.BMP")), 0 },
//	{ 'x', PIECEHV_BX, MEDIAFILE(_T("CBX.BMP")), 0 },
//	{ 's', PIECEHV_BS, MEDIAFILE(_T("CBS.BMP")), 0 },
//	{ 'k', PIECEHV_BK, MEDIAFILE(_T("CBK.BMP")), 0 },
//	{ 'p', PIECEHV_BP, MEDIAFILE(_T("CBP.BMP")), 0 },
//	{ 'b', PIECEHV_BB, MEDIAFILE(_T("CBB.BMP")), 0 },
//};

#ifdef ENGINE_CCE


PieceStruct CChessBoard::m_tPieceStructs[PIECE_NUM] = {
	{ 'J',  _T("CRJ.BMP"), 0 },
	{ 'M',  _T("CRM.BMP"), 0 },
	{ 'X',  _T("CRX.BMP"), 0 },
	{ 'S',  _T("CRS.BMP"), 0 },
	{ 'K',  _T("CRK.BMP"), 0 },
	{ 'P',  _T("CRP.BMP"), 0 },
	{ 'B',  _T("CRB.BMP"), 0 },
	{ 'j',  _T("CBJ.BMP"), 0 },
	{ 'm',  _T("CBM.BMP"), 0 },
	{ 'x',  _T("CBX.BMP"), 0 },
	{ 's',  _T("CBS.BMP"), 0 },
	{ 'k',  _T("CBK.BMP"), 0 },
	{ 'p',  _T("CBP.BMP"), 0 },
	{ 'b',  _T("CBB.BMP"), 0 },
};


#else

PieceStruct CChessBoard::m_tPieceStructs[PIECE_NUM] = {
	{ 'R',  _T("CRJ.BMP"), 0 },
	{ 'N',  _T("CRM.BMP"), 0 },
	{ 'B',  _T("CRX.BMP"), 0 },
	{ 'A',  _T("CRS.BMP"), 0 },
	{ 'K',  _T("CRK.BMP"), 0 },
	{ 'C',  _T("CRP.BMP"), 0 },
	{ 'P',  _T("CRB.BMP"), 0 },
	{ 'r',  _T("CBJ.BMP"), 0 },
	{ 'n',  _T("CBM.BMP"), 0 },
	{ 'b',  _T("CBX.BMP"), 0 },
	{ 'a',  _T("CBS.BMP"), 0 },
	{ 'k',  _T("CBK.BMP"), 0 },
	{ 'c',  _T("CBP.BMP"), 0 },
	{ 'p',  _T("CBB.BMP"), 0 },
};

#endif
CChessBoard::CChessBoard()
{
	m_pGameWindow = NULL;
	m_pChessEngine = NULL;
}

CChessBoard::~CChessBoard()
{
}


BOOL CChessBoard::LoadMedia()
{
	int i;
	HRESULT hr;

	try
	{
		for ( i = 0; i < PIECE_NUM; i++ )
		{
			if ( !m_tPieceStructs[i].pDib )
			{
				m_tPieceStructs[i].pDib = new CFastDIB();

				hr = m_tPieceStructs[i].pDib->LoadFromFile(
					AppEnv::GetMediaPath(m_tPieceStructs[i].szPieceFile));
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
			}
		}

		if ( !m_pBoardDIB )
		{
			m_pBoardDIB = new CFastDIB();
			if ( m_pBoardDIB )
			{
				hr = m_pBoardDIB->LoadFromFile(
					AppEnv::GetMediaPath(_T("CBOARD.BMP")));
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
			}
		}

		if ( !m_pBackgroundDIB )
		{
			m_pBackgroundDIB = new CFastDIB();
			if( m_pBackgroundDIB )
			{
				hr = m_pBackgroundDIB->LoadFromFile(
					AppEnv::szBackgoundBmp);
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
			}
		}

		if( !m_pMoveRectDIB )
		{
			m_pMoveRectDIB = new CFastDIB();
			if( m_pMoveRectDIB )
			{
				hr = m_pMoveRectDIB->LoadFromFile(
					AppEnv::GetMediaPath(_T("MOVERECT.BMP")));
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));				
			}
		}
	}
	catch(const char * e)
	{
		base::Log(1,e);
		CChessBoard::ReleaseMedia();
		return FALSE;
	}

	return TRUE;
}

void CChessBoard::ReleaseMedia()
{
	int i;
	for ( i = 0; i < PIECE_NUM; i++ )
	{
		if ( m_tPieceStructs[i].pDib )
		{
			delete m_tPieceStructs[i].pDib ;
			m_tPieceStructs[i].pDib = NULL;
		}
	}

	if ( m_pBoardDIB )
	{
		delete m_pBoardDIB ;
		m_pBoardDIB = NULL;
	}

	if ( m_pBackgroundDIB  )
	{
		delete m_pBackgroundDIB;
		m_pBackgroundDIB = NULL;
	}

	if( m_pMoveRectDIB )
	{
		delete m_pMoveRectDIB;
		m_pMoveRectDIB = NULL;
	}

}

void CChessBoard::DrawPiece( PieceStruct& ps, int squarex , int squarey )
{
	ps.pDib->Draw( g_pMainSurface, DRAWMODE_NORMAL, squarex * PIECE_DW, squarey * PIECE_DH );
}

void CChessBoard::DrawPiece(char piece, int x, int y)
{
	for(int i=0; i < PIECE_NUM; i++){
		if( CChessBoard::m_tPieceStructs[i].cPiece == piece)
		{
			DrawPiece(CChessBoard::m_tPieceStructs[i], x, y);
			break;
		}
	}
}

POINT CChessBoard::GetSquareOrigin(int squarex, int squarey )
{
	POINT pt;
	pt.x = squarex * PIECE_DW;
	pt.y = squarey * PIECE_DH;
	return pt;
}

void CChessBoard::ShowBestMove(CChessEngine::PieceMove * mv)
{
	assert(mv);
	assert(m_pGameWindow);


	GAMEWINDOWINFO gwi = m_pGameWindow->GetGameWindowInfo();

	if( gwi.bAvailible && !gwi.bGameOver && gwi.PlayerColor == gwi.Turn )
	{
		POINT ptSrc, ptDst;

		if( gwi.PlayerColor == TURN_WHITE )
		{
			ptSrc = this->GetSquareOrigin(mv->from.x, mv->from.y);
			ptDst = this->GetSquareOrigin(mv->to.x , mv->to.y );
		}
		else if( gwi.PlayerColor == TURN_BLACK )
		{
			ptSrc = this->GetSquareOrigin(mv->from.x, 9 - mv->from.y );
			ptDst = this->GetSquareOrigin(mv->to.x, 9 - mv->to.y );
		}

		static BOOL bShowSrcFocus = TRUE;
		
		bShowSrcFocus = !bShowSrcFocus;
		if( bShowSrcFocus )
		{
			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, ptSrc.x , ptSrc.y,0,0,0,0,255 ,1,0x00ff0000);
		} 
		else
		{
			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, ptDst.x, ptDst.y,0,0,0,0,255 ,1,0x00ff0000);
		}
	}
	
}

void CChessBoard::Update()
{
	assert( GetGameWindow() && GetChessEngine() );

	GetChessEngine()->UpdateState();

	GAMEWINDOWINFO gi ;
	WINDOWPLACEMENT wp;
	GetWindowPlacement( GetGameWindow()->GetFrameWindowHandle(),&wp);

	if (wp.showCmd == SW_SHOWNORMAL)
	{
		memset( &gi, 0, sizeof(gi));
		if( GetGameWindow()->ReadGameWindowInfo() )
		{
			gi = GetGameWindow()->GetGameWindowInfo();
			if( gi.bAvailible && !gi.bGameOver )
			{
				DrawBoard( &gi );

				if( gi.bObserverMode )
					EnableMenuItem(GetMenu(g_hWndMain),IDM_AUTOPLAY,MF_DISABLED);
				else
					EnableMenuItem(GetMenu(g_hWndMain),IDM_AUTOPLAY,MF_ENABLED);

				if( gi.PlayerColor != gi.Turn )
				{
					KillAlarm();
					GetChessEngine()->Stop();
					SetWindowText(g_hWndMain, _T("waiting..."));
				}
				else 
				{
					if( GetChessEngine()->GetState() == IChessEngine::Idle )
					{
						//it is player turn, and engine idle.
						IChessEngine::PieceMove * mv;
						mv = GetChessEngine()->GetBestMove();
						if ( !mv )
						{
							//there are no best move and engine idle, make a think...
							GetChessEngine()->Go(gi.szFen);
						}
						else 
						{
							//already have a bestmove then, make the move.
							SetWindowText(g_hWndMain, _T("Got a move"));
							ShowBestMove(mv);
							if( AppEnv::bAutoPlay && !gi.bObserverMode )
							{
								if ( mv->mvtimestamp == 0 )
								{
									mv->mvtimestamp = time(NULL);
									GetGameWindow()->MovePiece(mv);
								} 
								else
								{
									time_t now = time(NULL);
									if( now - mv->mvtimestamp > 1 )
									{
										SetAlarm();
									}
								}
							}
						}
					} 
					else 
					{
						// todo : draw thinking....
						SetWindowText(g_hWndMain, _T("thinking..."));
					}
				}
			}
		}
	} 
	else if ( wp.showCmd == SW_SHOWMINIMIZED )
	{
		DrawBoard( NULL );
	}

}

//
//void CChessBoard::ShowBestMove(int squarex1, int squarey1, int squarex2, int squarey2)
//{
//	if( gwi.bAvailible )
//	{
//		static BOOL bShowSrcRect = TRUE;
//		bShowSrcRect = !bShowSrcRect;
//		if( bShowSrcRect )
//		{
//			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, fx * PIECE_DW, fy * PIECE_DH,0,0,0,0,255 ,1,0x00ff0000);
//		} 
//		else
//		{
//			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, tx * PIECE_DW, ty * PIECE_DH,0,0,0,0,255 ,1,0x00ff0000);
//		}
//	}
//	
//}

void CChessBoard::DrawBoard(GAMEWINDOWINFO * gi)
{
	assert( g_pMainSurface && m_pBoardDIB )	;

	m_pBackgroundDIB->Draw(g_pMainSurface);
	m_pBoardDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL);

	if ( gi == NULL )
	{
		return;
	}

	char *lpFen = gi->szFen ;
	int x = 0;
	int y = 0;
	char c;

	c = *lpFen;

	while( c && c != ' ')
	{
		if( c >= '0' && c <= '9' )
		{
			x = x + c - '0';
		}
		else if( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') 
		{
			if( gi->PlayerColor == TURN_WHITE )
				DrawPiece( c , x, y );
			else if( gi->PlayerColor == TURN_BLACK)
				DrawPiece( c, x, 9 - y);
			x = x + 1;
		}else if( c == '/' )
		{
			x = 0;
			y ++;
		}

		c= *(++lpFen);
	}
}


/*
void CChessBoard::DrawBoard( IGameWindow * pgw )
{
	assert(pgw && g_pMainSurface && m_pBoardDIB);

	if( !pgw->GetHandle() )
	{
		return ;
	}

	int p;
	int x;
	int y;

	g_pMainSurface->ClearImage();
	m_pBoardDIB->Draw( g_pMainSurface );

	for( y =0; y < 10; y++ )
	{
		for( x = 0; x < 9; x ++)
		{
			p = pgw->GetPiece(x,y);
			if ( p )	this->DrawPiece(p, x, y);
		}
	}
	
	int turn = pgw->GetTurn();
	if( turn == 'b' )
	{
		this->DrawPiece( 'k' , 9, 1 );
	} else if ( turn == 'w' )
	{
		this->DrawPiece( 'K' , 9, 1 );
	}
}
*/