#include "common.h"
#include "QQNewChessWnd.h"

#define HASH_SEED		(1689)
#define SAMPLE_LEN	(20)
#define SAMPLE_SX	(15)
#define SAMPLE_SY	(15)

const TCHAR  * qq_classid = _T("Afx:400000:b:10003:6");


QncDlgItemInfo CQQNewChessWnd::QncItemsInfo[QNC_ITEMSIZE] = 
{
		{  0x960, _T("Button"), { 55, 31 } }, 
		{  0x961, _T("Button"), { 55, 31 } },
		{  0x962, _T("Button"), { 55, 31 } },
		{  0x963, _T("Button"), { 55, 31 } }, 
		{  0x964, _T("Button"), { 55, 31 } }, 
		{  0x965, _T("Button"), { 55, 31 } }, 
};



CQQNewChessWnd::CQQNewChessWnd(void)
{
	this->m_hwndMain = NULL;
	memset(this->m_hwndItems , 0, sizeof(this->m_hwndItems));
	ox = 242;
	oy = 43;
	dx = 57; 
	dy = 57;
	

}

CQQNewChessWnd::~CQQNewChessWnd(void)
{
}



BOOL CALLBACK QQNewChessWnd_EnumChildProc(  HWND hwnd,  LPARAM lParam)
{
	CQQNewChessWnd * pQncWnd = (CQQNewChessWnd *) lParam;

	WINDOWINFO wi;

	TCHAR buf[256];

	RealGetWindowClass(hwnd, buf, sizeof(buf));
	OutputDebugString(_T("ChildWindowClass="));
	OutputDebugString(buf);
	_stprintf_s( buf , _T(" CtrlID=0x%p"),  GetDlgCtrlID( hwnd ));
	OutputDebugString(buf);
	OutputDebugString(_T("\n"));

	int i;

	for( i = 0 ; i < QNC_ITEMSIZE; i++)
	{
		if ( GetDlgCtrlID( hwnd ) == CQQNewChessWnd::QncItemsInfo[i].nID )
		{
			memset(&wi, 0, sizeof(wi));

			GetWindowInfo(hwnd, &wi);
			_stprintf_s(buf, _T("Size(%d,%d)\n"), (wi.rcWindow.right - wi.rcWindow.left), (wi.rcWindow.bottom - wi.rcWindow.top));

			OutputDebugString(buf);
			if ( (wi.rcWindow.right - wi.rcWindow.left)  == CQQNewChessWnd::QncItemsInfo[i].ptSize.x 
				&& (wi.rcWindow.bottom - wi.rcWindow.top)  == CQQNewChessWnd::QncItemsInfo[i].ptSize.y )
			{
				pQncWnd->m_hwndItems[i] = hwnd;
			} 
		}
	}

	return true;
}


DWORD CQQNewChessWnd::GetHash( int x, int y )
{
	int sx = SAMPLE_SX;
	int sy = SAMPLE_SY;
	int len = SAMPLE_LEN;

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	DWORD h = HASH_SEED ^ (len * sizeof(DWORD));

	DWORD k;

	HDC hdc = GetDC(this->GetHwnd() );

	while( len )
	{
		k = GetPixel( hdc, ox + x * dx + sx , oy + y * dy + sy);

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h *= m; 
		h ^= k;

		sx ++;
		sy ++;
		len --;
	}

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

COLORREF * CQQNewChessWnd::GetRectData(int x, int y)
 {
	 COLORREF * color;
	 color = new COLORREF[58*58];
	 int count = 0;

	 if ( ! this->GetHwnd() )
	 {
		 return NULL;
	 }

	 HDC hdc = GetDC(this->GetHwnd() );

	 for( int i = 0 ; i < 58; i ++) 
		 for( int j = 0; j < 58 ; j ++ )
		 {
			 color[j*58+i] = GetPixel( hdc, ox + x * dx + i , oy + y * dy + j);
		 }

	 ReleaseDC(this->m_hwndMain, hdc);

	 return color;
 }

bool CQQNewChessWnd::Attach(HWND hwnd)
{
	int i;

	//OutputDebugString(_T("=============GetItems = Start\n"));

	RealGetWindowClass(hwnd, m_szWindowClass, sizeof(m_szWindowClass));
	//OutputDebugString(_T("WindowClass="));
	//OutputDebugString(m_szWindowClass);
	//OutputDebugString(_T("\n"));

	if ( wcsncmp( qq_classid, m_szWindowClass, wcslen(qq_classid) ) != 0)
	{
		return false;
	}

	EnumChildWindows(hwnd, QQNewChessWnd_EnumChildProc, (LPARAM) this);

	for( i = 0 ; i < QNC_ITEMSIZE; i++)
	{
		if ( this->m_hwndItems[i] == NULL )
		{
			return false;
		}
	}

	this->m_hwndMain = GetParent(m_hwndItems[0]);

	//OutputDebugString(_T("=============GetItems=== End\n"));
	return true;
}

BOOL CALLBACK MyEnumWindowsProc( HWND hwnd,    LPARAM lParam)
{
	CQQNewChessWnd * pQncWnd = (CQQNewChessWnd*) lParam;

	if( pQncWnd->Attach(hwnd) )
	{
		return FALSE;
	}
	
	return TRUE;
}

HWND CQQNewChessWnd::FindQQNewChessWindow()
{
	EnumWindows( MyEnumWindowsProc,(LPARAM)this);

	return (this->GetHwnd () );	
}
