#pragma once

#include "IGameWindow.h"


enum QncDlgItemEnum
{
	QNC_START_BTN	=	0,
	QNC_DRAWN_BTN	=	1,
	QNC_LOST_BTN		=	2,
	QNC_RETRACT		=  3,
	QNC_TOOLS_BTN	=	4,
	QNC_DISCS_BTN		=	5,
	QNC_ITEMSIZE		
};


struct QncDlgItemInfo
{
	INT			nID;
	LPTSTR		szWindowClass;
	POINT		ptSize;
};


class CQQNewChessWnd : public IGameWindow
{
	static QncDlgItemInfo QncItemsInfo[QNC_ITEMSIZE];

	TCHAR m_szWindowClass[256];

	HWND m_hwndItems[QNC_ITEMSIZE];

	HWND m_hwndMain;

	HWND m_hwndFrame;

	bool m_bGameOver;

	int	m_PlayerTurn;

	char m_fen[256];

	//GAMEWINDOWINFO m_gi;

public:
	 int ox ;
	 int oy ;

	 int dx ; 
	 int dy ;

private:

	bool ReadTurn(GAMEWINDOWINFO * pgi);
	bool ReadFen(GAMEWINDOWINFO * pgi);

	virtual char	GetPiece( int x, int y) ;
	virtual DWORD GetHashValue( int x, int y );

public:
	CQQNewChessWnd(void);
	~CQQNewChessWnd(void);

	COLORREF * CQQNewChessWnd::GetRectData(int x, int y);

	bool Attach(HWND hwnd);

	virtual BOOL MovePiece(IChessEngine::PieceMove * pmv);

	void DumpBoradHash();


	virtual bool ReadGameWindowInfo() ;

	virtual HWND GetHandle(void)
	{ 
		if ( m_hwndMain && IsWindow(m_hwndMain) )
		{
			return m_hwndMain;
		}
		m_hwndMain = NULL;
		return NULL; 
	}

	HWND GetFrameWnd() { return m_hwndFrame; }

	HWND FindQQNewChessWindow();
	DWORD CQQNewChessWnd::GetHashValue( int x, int y, int len );


	friend BOOL CALLBACK QQNewChessWnd_EnumChildProc( HWND hwnd,  LPARAM lParam);

};

