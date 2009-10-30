#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "QQNewChessWnd.h"

CQQNewChessWnd * g_pQcnWnd = NULL;

// GLOBAL VALUES
//___________________________________________________________________________

CFastDIB *g_pBoardDIB	= NULL;

PieceStruct g_tPieceStructs[PIECE_NUM] = {
	{ 'J', PIECEHV_RJ, MEDIAFILE(_T("CRJ.BMP")), 0 },
	{ 'M', PIECEHV_RM, MEDIAFILE(_T("CRM.BMP")), 0 },
	{ 'X', PIECEHV_RX, MEDIAFILE(_T("CRX.BMP")), 0 },
	{ 'S', PIECEHV_RS, MEDIAFILE(_T("CRS.BMP")), 0 },
	{ 'K', PIECEHV_RK, MEDIAFILE(_T("CRK.BMP")), 0 },
	{ 'P', PIECEHV_RP, MEDIAFILE(_T("CRP.BMP")), 0 },
	{ 'B', PIECEHV_RB, MEDIAFILE(_T("CRB.BMP")), 0 },
	{ 'j', PIECEHV_BJ, MEDIAFILE(_T("CBJ.BMP")), 0 },
	{ 'm', PIECEHV_BM, MEDIAFILE(_T("CBM.BMP")), 0 },
	{ 'x', PIECEHV_BX, MEDIAFILE(_T("CBX.BMP")), 0 },
	{ 's', PIECEHV_BS, MEDIAFILE(_T("CBS.BMP")), 0 },
	{ 'k', PIECEHV_BK, MEDIAFILE(_T("CBK.BMP")), 0 },
	{ 'p', PIECEHV_BP, MEDIAFILE(_T("CBP.BMP")), 0 },
	{ 'b', PIECEHV_BB, MEDIAFILE(_T("CBB.BMP")), 0 },
};


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


void LoadMedia()
{
	int i;
	HRESULT hr;
	for ( i = 0; i < PIECE_NUM; i++ )
	{
		if ( !g_tPieceStructs[i].pDib )
		{
			g_tPieceStructs[i].pDib = new CFastDIB();

			hr = g_tPieceStructs[i].pDib->LoadFromFile(g_tPieceStructs[i].szPieceFile);
			_CHECK(hr);
		}
	}

	if ( !g_pBoardDIB )
	{
		g_pBoardDIB = new CFastDIB();
		if ( g_pBoardDIB )
		{
			hr = g_pBoardDIB->LoadFromFile(MEDIAFILE(_T("CBOARD.BMP")));
			_CHECK(hr);
		}
	}
}

void ReleaseMedia()
{
	int i;
	for ( i = 0; i < PIECE_NUM; i++ )
	{
		if ( g_tPieceStructs[i].pDib )
		{
			delete g_tPieceStructs[i].pDib ;
			g_tPieceStructs[i].pDib = NULL;
		}
	}

	if ( g_pBoardDIB )
	{
		delete g_pBoardDIB ;
		g_pBoardDIB = NULL;
	}

}


void DrawPiece( PieceStruct& ps, int x , int y )
{
	ps.pDib->Draw( g_pMainSurface, 0, x * PIECE_DW, y * PIECE_DH );
}

void DrawBoard()
{
	if ( g_pMainSurface )
	{		
		g_pBoardDIB->Draw( g_pMainSurface );
		DrawPiece( g_tPieceStructs[0], 0, 0 );
		DrawPiece( g_tPieceStructs[1], 1, 1 );
		DrawPiece( g_tPieceStructs[2], 2, 2 );
		DrawPiece( g_tPieceStructs[3], 3, 3 );
		DrawPiece( g_tPieceStructs[4], 4, 4 );
		DrawPiece( g_tPieceStructs[5], 5, 5 );
		DrawPiece( g_tPieceStructs[6], 6, 6 );
		DrawPiece( g_tPieceStructs[7], 7, 7 );
		DrawPiece( g_tPieceStructs[8], 8, 8 );

		DrawPiece( g_tPieceStructs[9],  8, 0 );
		DrawPiece( g_tPieceStructs[10], 7, 1 );
		DrawPiece( g_tPieceStructs[11], 6, 2 );
		DrawPiece( g_tPieceStructs[12], 5, 3 );
		DrawPiece( g_tPieceStructs[13], 4, 4 );
	}
}

BOOL InitApp()
{
	LoadMedia();

	g_pQcnWnd = new CQQNewChessWnd();

	SetTimer(g_hWndMain,             // handle to main window 
		1,            // timer identifier 
		500,                 // 10-second interval 
		(TIMERPROC) NULL);     // no timer callback 


	return TRUE;
}

BOOL AppLoop()
{

	if( !g_pQcnWnd->GetHwnd() )
	{
		g_pQcnWnd->FindQQNewChessWindow();
	}

	DrawBoard();

	return TRUE;
}

BOOL ExitApp()
{

	if ( g_pQcnWnd ) delete g_pQcnWnd;

	ReleaseMedia();

	return TRUE;
}