#pragma once

#include "IGameWindow.h"
class CBitmapEx;


class CQQNewChessWnd : public IGameWindow
{
public :
	enum QncDlgItemEnum
	{
		QNC_START_BTN	=	0,
		QNC_DRAWN_BTN	=	1,
		QNC_LOST_BTN	=	2,
		QNC_RETRACT		=  3,
		QNC_TOOLS_BTN	=	4,
		QNC_DISCS_BTN	=	5,
		QNC_ITEMSIZE		
	};


	struct QncDlgItemInfo
	{
		INT			nID;
		LPTSTR		szWindowClass;
		POINT		ptSize;
	};

protected:
	static QncDlgItemInfo _QncItemsInfo[QNC_ITEMSIZE];

	TCHAR m_szWindowClass[256];

	HWND m_hwndItems[QNC_ITEMSIZE];

	HWND m_hwndMain;

	HWND m_hwndFrame;

	bool m_bGameOver;

	int	m_PlayerTurn;

	char m_fen[256];

	CBitmapEx * m_pBmpGameWindow;


private:

	int TURN1_X;
	int TURN1_Y;
	int TURN2_X;
	int TURN2_Y;
	int TURN_WHITE_KEY;
	int TURN_BLACK_KEY;

	POINT m_ptBoardOrigin;
	SIZE  m_sizeSquare;
	POINT m_ptSampleOrigin;
	int   m_nSampleLen;

	POINT GetSquareOrigin(int squarex, int squarey)
	{
		POINT pt;
		pt.x = m_ptBoardOrigin.x + squarex * m_sizeSquare.cx;
		pt.y = m_ptBoardOrigin.y + squarey * m_sizeSquare.cy;
		return pt;
	}

private:

	bool ReadTurn(GAMEWINDOWINFO * pgi);
	bool ReadFen(GAMEWINDOWINFO * pgi);

	virtual char	GetPiece( int squarex, int squarey) ;
	virtual DWORD GetHashValue( int squarex, int squarey );

public:
	CQQNewChessWnd(void);
	~CQQNewChessWnd(void);

	COLORREF * CQQNewChessWnd::GetRectData(int x, int y);

	bool Attach(HWND hwnd);

	virtual BOOL MovePiece(IChessEngine::PieceMove * pmv);

	void DumpBoradHash();

	bool LoadHashValue(TCHAR * szFile);

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

	virtual HWND GetFrameWindowHandle() { return m_hwndFrame; }

	HWND FindGameWindow();

	DWORD CQQNewChessWnd::GetHashValue( int x, int y, int len );

	virtual HWND SearchGameWindow();

	friend BOOL CALLBACK QQNewChessWnd_EnumChildProc( HWND hwnd,  LPARAM lParam);

};

