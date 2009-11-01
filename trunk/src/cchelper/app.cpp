#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"

using namespace base;

CQQNewChessWnd	* g_pQcnWnd = NULL;
CChessEngine	* g_pChessEngine = NULL;

// GLOBAL VALUES
//___________________________________________________________________________

// CChessBoard
//___________________________________________________________________________
CFastDIB * CChessBoard::m_pBoardDIB = NULL;
CFastDIB * CChessBoard::m_pMoveRectDIB = NULL;

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

PieceStruct CChessBoard::m_tPieceStructs[PIECE_NUM] = {
	{ 'J',  MEDIAFILE(_T("CRJ.BMP")), 0 },
	{ 'M',  MEDIAFILE(_T("CRM.BMP")), 0 },
	{ 'X',  MEDIAFILE(_T("CRX.BMP")), 0 },
	{ 'S',  MEDIAFILE(_T("CRS.BMP")), 0 },
	{ 'K',  MEDIAFILE(_T("CRK.BMP")), 0 },
	{ 'P',  MEDIAFILE(_T("CRP.BMP")), 0 },
	{ 'B',  MEDIAFILE(_T("CRB.BMP")), 0 },
	{ 'j',  MEDIAFILE(_T("CBJ.BMP")), 0 },
	{ 'm',  MEDIAFILE(_T("CBM.BMP")), 0 },
	{ 'x',  MEDIAFILE(_T("CBX.BMP")), 0 },
	{ 's',  MEDIAFILE(_T("CBS.BMP")), 0 },
	{ 'k',  MEDIAFILE(_T("CBK.BMP")), 0 },
	{ 'p',  MEDIAFILE(_T("CBP.BMP")), 0 },
	{ 'b',  MEDIAFILE(_T("CBB.BMP")), 0 },
};

CChessBoard::CChessBoard()
{
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

				hr = m_tPieceStructs[i].pDib->LoadFromFile(m_tPieceStructs[i].szPieceFile);
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
			}
		}

		if ( !m_pBoardDIB )
		{
			m_pBoardDIB = new CFastDIB();
			if ( m_pBoardDIB )
			{
				hr = m_pBoardDIB->LoadFromFile(MEDIAFILE(_T("CBOARD.BMP")));
				THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
			}
		}

		if( !m_pMoveRectDIB )
		{
			m_pMoveRectDIB = new CFastDIB();
			if( m_pMoveRectDIB )
			{
				hr = m_pMoveRectDIB->LoadFromFile(MEDIAFILE(_T("MOVERECT.BMP")));
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

	if( m_pMoveRectDIB )
	{
		delete m_pMoveRectDIB;
		m_pMoveRectDIB = NULL;
	}

}

void CChessBoard::DrawPiece( PieceStruct& ps, int x , int y )
{
	ps.pDib->Draw( g_pMainSurface, 0, x * PIECE_DW, y * PIECE_DH );
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

void CChessBoard::ShowBestMove(int fx, int fy, int tx, int ty)
{
	m_pMoveRectDIB->Draw( g_pMainSurface, 0, fx * PIECE_DW, fy * PIECE_DH);
	m_pMoveRectDIB->Draw( g_pMainSurface, 0, tx * PIECE_DW, ty * PIECE_DH);
}


void CChessBoard::DrawBoard(GAMEINFO * gi)
{
	assert(gi);

	char *lpFen = gi->szFen ;
	int x = 0;
	int y = 0;
	char c;

	if ( g_pMainSurface )
	{		
		m_pBoardDIB->Draw( g_pMainSurface );

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

CChessBoard * g_pBoard = NULL;

BOOL InitApp()
{
	if( !CChessBoard::LoadMedia() )
	{
		return FALSE;
	}

	g_pQcnWnd = new CQQNewChessWnd();

	g_pBoard = new CChessBoard();

	g_pChessEngine = new CChessEngine();

	g_pChessEngine->InitEngine("cce.exe");

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
	} 
	else
	{
		GAMEINFO gi;
		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd,&wp);

		if (wp.showCmd == SW_SHOWNORMAL)
		{
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
							if( g_pChessEngine->GetState() == CChessEngine::BusyWait  )
							{
								g_pChessEngine->SendCommand("stop");
								Sleep(20);
							}
							sprintf(szCmd, "position fen %s", gi.szFen );
							g_pChessEngine->SendCommand(szCmd);
							Sleep(20);
							g_pChessEngine->SendCommand("go time 3000" );
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
	//g_pBoard->DrawBoard("jmxsksxmj/9/1p5p1/b1b1b1b1b/9/9/B1B1B1B1B/1P5P1/9/JMXSKSXMJ w");
	
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