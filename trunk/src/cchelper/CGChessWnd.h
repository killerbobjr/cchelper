#pragma once

#include "IGameWindow.h"
class CBitmapEx;

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


class CGChessWnd : public IGameWindow
{
public :

protected:

	HWND m_hwndMain;

	HWND m_hwndFrame;

	bool m_bGameOver;

	int	m_PlayerTurn;

	char m_fen[256];

	CBitmapEx * m_pBmpGameWindow;


private:

	BOOL m_bAttachFound;

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
	unsigned int m_uWindowKey;
	unsigned int m_uDisplayModeFormat;

	POINT GetSquareOrigin(int squarex, int squarey)
	{
		POINT pt;
		pt.x = m_ptBoardOrigin.x + squarex * m_sizeSquare.cx;
		pt.y = m_ptBoardOrigin.y + squarey * m_sizeSquare.cy;
		return pt;
	}

private:
	void CGChessWnd::GetBmpFromD3D(HWND hwnd);
	void CGChessWnd::GetBmpFromGDI(HWND hwnd);

	bool ReadTurn(GAMEWINDOWINFO * pgi);
	bool ReadFen(GAMEWINDOWINFO * pgi);

	virtual char	GetPiece( int squarex, int squarey) ;
	virtual DWORD GetHashValue( int squarex, int squarey );

public:
	CGChessWnd(void);
	~CGChessWnd(void);

	COLORREF * CGChessWnd::GetRectData(int x, int y);

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

	virtual HWND SearchGameWindow(void);

	virtual HWND GetFrameWindowHandle() { return m_hwndFrame; }

	DWORD CGChessWnd::GetHashValue( int x, int y, int len );


	friend BOOL CALLBACK CGChessWnd_EnumChildProc( HWND hwnd,  LPARAM lParam);

};

