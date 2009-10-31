#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "QQNewChessWnd.h"

using namespace base;

CQQNewChessWnd * g_pQcnWnd = NULL;

// GLOBAL VALUES
//___________________________________________________________________________

// CChessBoard
//___________________________________________________________________________
CFastDIB * CChessBoard::m_pBoardDIB = NULL;
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

void CChessBoard::DrawBoard(const char * lpFen)
{
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
				DrawPiece( c , x, y );
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

CChessBoard * g_pBoard = NULL;

BOOL InitApp()
{
	if( !CChessBoard::LoadMedia() )
	{
		return FALSE;
	}

	g_pQcnWnd = new CQQNewChessWnd();

	g_pBoard = new CChessBoard();

	return TRUE;
}

BOOL AppLoop()
{
	HWND hwnd = g_pQcnWnd->GetHandle();
	if( !hwnd )
	{
		g_pQcnWnd->FindQQNewChessWindow();
	} 
	else
	{
		WINDOWPLACEMENT wp;
		GetWindowPlacement(hwnd,&wp);

		if (wp.showCmd == SW_SHOWNORMAL){
			g_pBoard->DrawBoard( g_pQcnWnd );
			
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

	CChessBoard::ReleaseMedia();

	return TRUE;
}