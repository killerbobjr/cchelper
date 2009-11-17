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


const TCHAR  * _CGChessWnd_classid = _T("ClientFrame_CMainFrame");

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

CGChessWnd::QncDlgItemInfo CGChessWnd::_QncItemsInfo[QNC_ITEMSIZE] = 
{
		{  0x3EB, _T("Button"), { 68, 30 } }, 
		{  0x3f9, _T("Button"), { 68, 30 } },
		{  0x3fa, _T("Button"), { 68, 30 } },
		{  0x429, _T("Button"), { 68, 30 } }, 
		{  0x42f, _T("Button"), { 68, 30 } }, 
		{  0x447, _T("Button"), { 68, 30 } }, 
};
//
//enum QncDlgItemEnum
//{
//	QNC_START_BTN	=	0,
//	QNC_DRAWN_BTN	=	1,
//	QNC_LOST_BTN	=	2,
//	QNC_RETRACT		=  3,
//	QNC_TOOLS_BTN	=	4,
//	QNC_DISCS_BTN	=	5,
//	QNC_ITEMSIZE		
//};

CGChessWnd::CGChessWnd(void)
{
	this->m_hwndMain = NULL;
	memset(this->m_hwndItems , 0, sizeof(this->m_hwndItems));
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

	BOOL bStartEnable, bDrawnEnable, bLostEnable, bRetractEnable, bToolsEnable, bDiscsEnable;

	bStartEnable	= IsWindowEnabled(m_hwndItems[QNC_START_BTN]);
	bDrawnEnable	= IsWindowEnabled(m_hwndItems[QNC_DRAWN_BTN]);
	bLostEnable		= IsWindowEnabled(m_hwndItems[QNC_LOST_BTN]);
	bRetractEnable	= IsWindowEnabled(m_hwndItems[QNC_RETRACT]);
	bToolsEnable	= IsWindowEnabled(m_hwndItems[QNC_TOOLS_BTN]);
	bDiscsEnable	= IsWindowEnabled(m_hwndItems[QNC_DISCS_BTN]);


	if(!bStartEnable && !bDrawnEnable && !bLostEnable && !bRetractEnable && !bToolsEnable && bDiscsEnable)
	{
		gwi.bObserverMode = true;
	} 
	else
	{
		gwi.bObserverMode = false;
	}
	
	if( ReadFen(&gwi) )
	{
 		if(bStartEnable || 
			(!bStartEnable && !bDrawnEnable && !bLostEnable && !bRetractEnable && !bToolsEnable && !bDiscsEnable ) )
		{
			if( !m_bGameOver )
			{
				m_bGameOver = true;
				if( m_pChessEngine )
				{
					m_pChessEngine->GameOver();
				}
			}
		}
		else
		{
			if ( m_bGameOver )
			{
				m_bGameOver = false;
				if( m_pChessEngine )
				{
					m_pChessEngine->Restart();
				}
			}
		}

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

BOOL CALLBACK CGChessWnd_EnumChildProc(  HWND hwnd,  LPARAM lParam)
{
	CGChessWnd * pQncWnd = (CGChessWnd *) lParam;

	bool bAllFound = true;

	WINDOWINFO wi;

	TCHAR buf[256];


	_stprintf_s( buf , _T(" CtrlID=0x%p\n"),  GetDlgCtrlID( hwnd ));
	OutputDebugString(buf);

	int i;

	for( i = 0 ; i < CGChessWnd::QNC_ITEMSIZE; i++)
	{
		if ( GetDlgCtrlID( hwnd ) == CGChessWnd::_QncItemsInfo[i].nID )
		{
			memset(&wi, 0, sizeof(wi));

			GetWindowInfo(hwnd, &wi);
			_stprintf_s(buf, _T("Size(%d,%d)\n"), (wi.rcWindow.right - wi.rcWindow.left), (wi.rcWindow.bottom - wi.rcWindow.top));

			if ( (wi.rcWindow.right - wi.rcWindow.left)  == CGChessWnd::_QncItemsInfo[i].ptSize.x 
				&& (wi.rcWindow.bottom - wi.rcWindow.top)  == CGChessWnd::_QncItemsInfo[i].ptSize.y )
			{
				pQncWnd->m_hwndItems[i] = hwnd;
			} 
		}
		if( !pQncWnd->m_hwndItems[i] )
			bAllFound = false;
	}

	if( !bAllFound )
	{
		EnumChildWindows(hwnd, CGChessWnd_EnumChildProc, (LPARAM) pQncWnd);
		if( pQncWnd->m_bAttachFound )
		{
			return FALSE;
		}
		return TRUE;
	}else{
		pQncWnd->m_bAttachFound = TRUE;
		return FALSE;
	}
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

	//SendMessage(this->GetFrameWindowHandle(), WM_PARENTNOTIFY,WM_LBUTTONDOWN,MAKELONG(ptSrc.x+sx,ptSrc.y+sy));
	//SendMessage(this->GetFrameWindowHandle(), WM_MOUSEACTIVATE,(WPARAM)GetFrameWindowHandle(),MAKELONG(HTCLIENT,WM_LBUTTONDOWN));

	ptSrc.x += rt.left + sx;
	ptSrc.y += rt.top + sy;

	ptDst = this->GetSquareOrigin( tx, ty );
	//SendMessage(this->GetFrameWindowHandle(), WM_PARENTNOTIFY,WM_LBUTTONDOWN,MAKELONG(ptDst.x+sx,ptDst.y+sy));
	//SendMessage(this->GetFrameWindowHandle(), WM_MOUSEACTIVATE,(WPARAM)GetFrameWindowHandle(),MAKELONG(HTCLIENT,WM_LBUTTONDOWN));

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


void CGChessWnd::DumpBoradHash()
{
	char buf[1024];

	DWORD k;
	int i;
	int j;

	for( j =0; j < 10; j++ )
	{
		for( i = 0; i < 9; i ++)
		{
			k = GetHashValue(i,j);
			sprintf_s(buf, "[%4p]", k);
			base::Log(1, buf );
		}
	}
}

bool CGChessWnd::Attach(HWND hwnd)
{

	this->m_bAttachFound = FALSE;

	TCHAR szWindowClass[256];

	RealGetWindowClass(hwnd, szWindowClass, sizeof(szWindowClass));

	m_hwndMain = NULL;
	m_hwndFrame = NULL;
	if(capture::GetWindowKey(hwnd) == this->m_uWindowKey )
	{
		m_hwndMain =hwnd;
		m_hwndFrame = hwnd;
		return true;
	}

	EnumChildWindows(hwnd, CGChessWnd_EnumChildProc, (LPARAM) this);


	memset(&m_GameWindowInfo, 0, sizeof(m_GameWindowInfo));

	return true;
}

BOOL CALLBACK CGChessWnd_MyEnumWindowsProc( HWND hwnd,    LPARAM lParam)
{
	CGChessWnd * pQncWnd = (CGChessWnd*) lParam;

	if( pQncWnd->Attach(hwnd) )
	{
		return FALSE;
	}
	
	return TRUE;
}

HWND CGChessWnd::FindGameWindow()
{
	EnumWindows( CGChessWnd_MyEnumWindowsProc,(LPARAM)this);

	return (this->GetHandle () );	
}
