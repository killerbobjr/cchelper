#include <string>
#include <fstream>
#include "json/json.h"

#include "common.h"
#include "CGChessWnd.h"
#include "murmurhash.h"
#include "capture.h"

using namespace std;

#define BUTTON_ID_CANCEL	(2)
#define BUTTON_ID_OK		(1)

#define PIECE_NUM	(14)


struct PieceHashValue
{
	char cPiece;
	DWORD dwPieceHashValue;
};

PieceHashValue _CGChessWnd_PieceHashValues[PIECE_NUM] = {
	{ 'R', 0 },
	{ 'N', 0 },
	{ 'B', 0 },
	{ 'A', 0 },
	{ 'K', 0 },
	{ 'C', 0 },
	{ 'P', 0 },
	{ 'r', 0 },
	{ 'n', 0 },
	{ 'b', 0 },
	{ 'a', 0 },
	{ 'k', 0 },
	{ 'c', 0 },
	{ 'p', 0 },
};


CGChessWnd::CGChessWnd(void)
{
	this->m_hwndMain = NULL;

	m_ptBoardOrigin.x = 242;
	m_ptBoardOrigin.y = 43;
	m_sizeSquare.cx = 57; 
	m_sizeSquare.cy = 57;


	m_bGameOver = true;
	m_pChessEngine = NULL;
	m_hwndFrame = NULL;

	m_pBmpGameWindow = NULL;

}

CGChessWnd::~CGChessWnd(void)
{
	if( m_pBmpGameWindow )
		delete m_pBmpGameWindow;
}

bool CGChessWnd::LoadHashValue(TCHAR * szFile)
{	
	try
	{
		Json::Value  vRoot;

		std::ifstream fs(szFile);
		if(fs)
		{			
			Json::Reader reader;
			reader.parse(fs, vRoot, false);

			m_ptBoardOrigin.x = vRoot["BoardOriginX"].asInt();
			m_ptBoardOrigin.y = vRoot["BoardOriginY"].asInt();
			m_sizeSquare.cx = vRoot["SquareWidth"].asInt();
			m_sizeSquare.cy = vRoot["SquareHeight"].asInt();
			m_ptSampleOrigin.x = vRoot["SampleOriginX"].asInt();
			m_ptSampleOrigin.y = vRoot["SampleOriginY"].asInt();
			m_nSampleLen = vRoot["SampleLen"].asInt();
			m_uWindowKey = vRoot["WindowKey"].asUInt();
			m_uDisplayModeFormat = vRoot["DisplayModeFormat"].asUInt();

		
			TURN1_X			=vRoot["TURN1_X"].asInt();
			TURN1_Y			=vRoot["TURN1_Y"].asInt();
			TURN2_X			=vRoot["TURN2_X"].asInt();
			TURN2_Y			=vRoot["TURN2_Y"].asInt();
			TURN_WHITE_KEY	=vRoot["TURN_WHITE_KEY"].asUInt();
			TURN_BLACK_KEY	=vRoot["TURN_BLACK_KEY"].asUInt();

			Json::Value vPieceHashTable;

			vPieceHashTable = vRoot["PieceHashTable"];

			int size = vPieceHashTable.size() ;
			assert(size == PIECE_NUM);

			for ( int index =0; index < size; ++index )
			{
				_CGChessWnd_PieceHashValues[index].cPiece = vPieceHashTable[index]["PieceKey"].asUInt() ;
				_CGChessWnd_PieceHashValues[index].dwPieceHashValue = vPieceHashTable[index]["PieceHash"].asUInt() ;
			}

		}
		fs.close();
		

		
		return true;
	}
	catch(std::exception& e)
	{
		base::Log(0, e.what() );
		return false;
	}
}


void CGChessWnd::GetBmpFromD3D(HWND hwnd)
{
	LPBYTE pData;
	ScreenCapture cap;
	if(cap.CaptureByD3D(hwnd, &pData))
	{
		assert(cap.GetDisplayModeFormat() == this->m_uDisplayModeFormat );
		m_pBmpGameWindow->Load(pData);
		free(pData);
	}
}


void CGChessWnd::GetBmpFromGDI(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);
	if(hdc)
	{
		HBITMAP hbmp = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP );
		if(hbmp)
		{
			m_pBmpGameWindow->Load(hbmp);

			if( !m_pBmpGameWindow->IsValid())
			{
				DeleteObject(hbmp);
				ReleaseDC(hwnd, hdc);
			}
			DeleteObject(hbmp);
		}
		ReleaseDC(hwnd,hdc);
	}
}


bool CGChessWnd::ReadGameWindowInfo()
{
	HWND hwnd = GetHandle();
	if( !hwnd )
	{
		return false;
	}

	if( !m_pBmpGameWindow )
	{
		m_pBmpGameWindow = new CBitmapEx();
	}

	//GetBmpFromGDI(hwnd);
	GetBmpFromD3D(hwnd);

	GAMEWINDOWINFO gwi;

	memset(&gwi, 0, sizeof(gwi));

	bool ret = false;
	
	if( ReadFen(&gwi) )
	{
		if( m_GameWindowInfo.bAvailible )
		{
			if(strcmp(m_GameWindowInfo.szFen, gwi.szFen ) != 0 )
			{
				if( m_GameWindowInfo.Turn != gwi.Turn )
				{
					gwi.bTurnChanged = true;
					gwi.bAvailible = true;
				} 
				else
				{
					gwi.bAvailible = false;
				}
			}
			else
			{
				if( m_GameWindowInfo.Turn != gwi.Turn )
				{
					gwi.bAvailible = false;					
				}else
				{
					gwi.bAvailible = true;
				}
			}
		} 
		else 
		{
			gwi.bAvailible = true;
		}
	} 
	else
	{
		gwi.bAvailible = false;
	}

	m_GameWindowInfo = gwi;

	return m_GameWindowInfo.bAvailible ;
}

bool CGChessWnd::ReadTurn(GAMEWINDOWINFO * pgi)
{
	pgi->Turn = TURN_NONE ;
	pgi->PlayerColor = TURN_NONE;

	DWORD key1 = 0;
	DWORD key2 = 0;

	key1 = GetHashValue( TURN1_X, TURN1_Y, m_nSampleLen);
	key2 = GetHashValue( TURN2_X, TURN2_Y, m_nSampleLen);

	if( key1 == TURN_WHITE_KEY )
	{
		pgi->Turn  = TURN_WHITE ;
		pgi->PlayerColor = TURN_BLACK;
	} else if (key2 == TURN_WHITE_KEY ) {
		pgi->Turn  = TURN_WHITE ;
		pgi->PlayerColor = TURN_WHITE;		
	} else if( key1 == TURN_BLACK_KEY ) {
		pgi->Turn  = TURN_BLACK;
		pgi->PlayerColor = TURN_WHITE;
	} else if ( key2 == TURN_BLACK_KEY)	{
		pgi->Turn  = TURN_BLACK ;
		pgi->PlayerColor = TURN_BLACK;
	}

	if ( pgi->Turn == TURN_NONE )
		return false;

	return true;
}


bool CGChessWnd::ReadFen(GAMEWINDOWINFO * pgi)
{
	assert(pgi);

	if ( !ReadTurn(pgi) )
	{
		pgi->szFen[0] = 0;
		return false;
	}
	
	char * pFen = pgi->szFen ;

	int p;
	int x;
	int y;

	int count = 0;

	for( y =0; y < 10; y++ )
	{
		for( x = 0; x < 9; x ++)
		{
			if( pgi->PlayerColor == TURN_WHITE) p = GetPiece(x,y);
			else if(pgi->PlayerColor == TURN_BLACK) p = GetPiece(x, 9 - y );
			if ( p )
			{
				if ( count > 0 )
				{
					*pFen++ = '0' + count;
					count = 0;
				}
				*pFen++ = p;
			}
			else
			{
				count ++;
			}
		}
		if ( count > 0 )
		{
			*pFen++ = '0' + count;
			count = 0;
		}
		if( y < 9 )
			*pFen++ = '/';
	}
	*pFen++ = ' ';
	*pFen++ = pgi->Turn ;
	*pFen++ = 0;

	return true;
}

BOOL CGChessWnd::MovePiece(IChessEngine::PieceMove * pmv )
{
	assert(pmv);

	HWND hWnd = this->m_hwndMain ;
	
	if( !hWnd )	return FALSE;

	int fx, fy, tx, ty;

	fx = pmv->from.x ;
	fy = pmv->from.y ;
	tx = pmv->to.x ;
	ty = pmv->to.y ;

	if( this->GetGameWindowInfo().PlayerColor == TURN_BLACK) 
	{
		fy = 9 - fy;
		ty = 9 - ty;
	}

	RECT rt;

	GetWindowRect(hWnd, &rt);

	int sx = m_sizeSquare.cx / 2;
	int sy = m_sizeSquare.cy / 2;

	POINT ptSave;

	POINT ptSrc, ptDst;

	GetCursorPos(&ptSave);

	ptSrc = this->GetSquareOrigin( fx, fy );

	ptSrc.x += rt.left + sx;
	ptSrc.y += rt.top + sy;

	ptDst = this->GetSquareOrigin( tx, ty );

	ptDst.x += rt.left + sx;
	ptDst.y += rt.top + sy;

	HWND hwnd1 = WindowFromPoint(ptSrc);
	HWND hwnd2 = WindowFromPoint(ptDst);
	if( hwnd2 != this->m_hwndMain || hwnd1 != this->m_hwndMain   )
	{
		return FALSE;
	}


	INPUT in[2];

	memset(in, 0, sizeof(INPUT) * 2);

	// click the piece position.
	in[0].type = INPUT_MOUSE;
	in[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	in[0].mi.dx = ptSrc.x ;
	in[0].mi.dy = ptSrc.y;

	in[1].type = INPUT_MOUSE;
	in[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	in[1].mi.dx = ptSrc.x ;
	in[1].mi.dy = ptSrc.y;

	SetCursorPos( ptSrc.x , ptSrc.y );	
	SendInput(2, in, sizeof(INPUT));


	// click the taget position.
	in[0].type = INPUT_MOUSE;
	in[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	in[0].mi.dx = ptDst.x;
	in[0].mi.dy = ptDst.y;

	in[1].type = INPUT_MOUSE;
	in[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
	in[1].mi.dx = ptDst.x ;
	in[1].mi.dy = ptDst.y ;

	SetCursorPos( ptDst.x, ptDst.y );
	SendInput(2, in, sizeof(INPUT));

	SetCursorPos( ptSave.x, ptSave.y );


	return TRUE;

}


DWORD CGChessWnd::GetHashValue( int x, int y, int len )
{
	assert( m_pBmpGameWindow );

	if( !this->m_pBmpGameWindow->IsValid() || !this->GetHandle() )
		return 0;

	DWORD *pSampleData = new DWORD[len];

	DWORD k;

	for( int i = 0; i < len; i ++)
	{
		pSampleData[i] = m_pBmpGameWindow->GetPixel( x++, y++);
	}

	k = base::MurmurHash2(pSampleData, len * sizeof(DWORD));
	
	delete pSampleData;

	return k;
}


DWORD CGChessWnd::GetHashValue( int sqx, int sqy )
{
	POINT ptSquare;

	ptSquare = this->GetSquareOrigin( sqx, sqy );

	return GetHashValue( ptSquare.x + m_ptSampleOrigin.x, ptSquare.y + m_ptSampleOrigin.y, m_nSampleLen );
}

char CGChessWnd::GetPiece( int sqx, int sqy)
{
	char ret = 0;
	DWORD k;
	k = GetHashValue(sqx,sqy);

	for( int i=0; i < PIECE_NUM; i++) {
		if ( _CGChessWnd_PieceHashValues[i].dwPieceHashValue == k )
		{
			ret = _CGChessWnd_PieceHashValues[i].cPiece ;
			break;
		}
	}

	return ret;
}


HWND CGChessWnd::SearchGameWindow(void)
{
	ScreenCapture cap;
	//cap.GetWindowKey(g_hWndMain);

	TCHAR szTitle[256];

	sprintf(szTitle, "HWND[%x]:%u", g_hWndMain, cap.GetWindowKey(g_hWndMain));

	SetWindowText(g_hWndMain, szTitle);


	return NULL;//return cap.SearchWindow(this->m_uWindowKey,true);
}


//
//bool CGChessWnd::Attach(HWND hwnd)
//{
//	if( !m_uWindowKey ) 
//		return false;
//
//	if(capture::GetWindowKey(hwnd) == m_uWindowKey )
//	{
//		m_hwndMain = hwnd;
//		m_hwndFrame = hwnd;
//	}
//	else
//	{
//		//EnumChildWindows(NULL, CGChessWnd_EnumChildProc, (LPARAM)this);
//	}
//
//	memset(&m_GameWindowInfo, 0, sizeof(m_GameWindowInfo));
//
//	if(this->GetHandle())		
//		return true;
//	else 
//		return false;
//}
//
//BOOL CALLBACK CGChessWnd_EnumChildProc(  HWND hwnd,  LPARAM lParam)
//{
//	CGChessWnd * pWnd = (CGChessWnd *) lParam;
//
//	if( pWnd->GetHandle() )
//		return FALSE;
//
//	if( capture::GetWindowKey(hwnd) == pWnd->m_uWindowKey)
//	{
//		pWnd->m_hwndFrame = hwnd;
//		pWnd->m_hwndMain = hwnd;
//		return FALSE;
//	}
//	else
//	{
////		EnumChildWindows(hwnd, CGChessWnd_EnumChildProc, lParam);
//	}
//
//	if( pWnd->GetHandle() )
//		return FALSE;
//	else
//		return TRUE;
//}

