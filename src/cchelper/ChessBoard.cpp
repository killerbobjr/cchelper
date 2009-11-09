#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"
#include "ChessEngine.h"
#include "ChessBoard.h"
#include "AppEnv.h"

using namespace base;

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

	if( m_pMoveRectDIB )
	{
		delete m_pMoveRectDIB;
		m_pMoveRectDIB = NULL;
	}

}

void CChessBoard::DrawPiece( PieceStruct& ps, int x , int y )
{
	ps.pDib->Draw( g_pMainSurface, DRAWMODE_NORMAL, x * PIECE_DW, y * PIECE_DH );
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
	static BOOL bShowSrcRect = TRUE;
	bShowSrcRect = !bShowSrcRect;
	if( bShowSrcRect )
	{
		m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, fx * PIECE_DW, fy * PIECE_DH,0,0,0,0,255 ,1,0x00ff0000);
	} 
	else
	{
		m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, tx * PIECE_DW, ty * PIECE_DH,0,0,0,0,255 ,1,0x00ff0000);
	}
}

void CChessBoard::DrawBoard(GAMEWINDOWINFO * gi)
{
	assert( g_pMainSurface && m_pBoardDIB )	;

	if ( gi == NULL )
	{
		m_pBoardDIB->Draw( g_pMainSurface );
		return;
	}

	char *lpFen = gi->szFen ;
	int x = 0;
	int y = 0;
	char c;

	c = *lpFen;

	m_pBoardDIB->Draw( g_pMainSurface );
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