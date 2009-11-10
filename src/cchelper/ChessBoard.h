#pragma once

#include "IGameWindow.h"
#include "IChessEngine.h"

// DEFINES
//___________________________________________________________________________s

#define PIECE_W		(18)
#define PIECE_H		(18)
#define PIECE_DW	(24)
#define PIECE_DH	(24)


#define PIECE_NUM	(14)

#define MAX_LOADSTRING	256

#define THROW_CHECK(hr,s) if (hr != S_OK) throw(s);

// STRUCTS
//___________________________________________________________________________

struct PieceStruct
{
	char		cPiece;
	//DWORD		dwPieceHashValue;
	TCHAR		* szPieceFile;
	CFastDIB	* pDib;
};


// @description
// Draw helper chess board.
class CChessBoard
{
	static CFastDIB		*m_pBoardDIB;
	static CFastDIB		*m_pBackgroundDIB;
	static CFastDIB		*m_pMoveRectDIB;
	static PieceStruct	m_tPieceStructs[PIECE_NUM];

public:
	static BOOL LoadMedia();
	static void ReleaseMedia();

private:

	IGameWindow * m_pGameWindow;
	IChessEngine * m_pChessEngine;

private:
	void DrawPiece( PieceStruct& ps, int squarex , int squarey );
	POINT GetSquareOrigin(int squarex, int squarey );

public:
	CChessBoard();
	~CChessBoard();

	void SetGameWindow(IGameWindow * pgw)
	{
		m_pGameWindow = pgw;
	}

	void SetChessEngine(IChessEngine * pce)
	{
		m_pChessEngine = pce;
	}

	IChessEngine * GetChessEngine()
	{
		return m_pChessEngine;
	}

	IGameWindow * GetGameWindow()
	{
		if( m_pGameWindow && m_pGameWindow->GetHandle())
		{
			return m_pGameWindow;
		} 
		else
		{
			m_pGameWindow = NULL;
			return NULL;
		}
	}

	void Update();

	void ShowBestMove(CChessEngine::PieceMove * mv);
	void ShowBestMove(int squarex1, int squarey1, int squarex2, int squarey2);

	void DrawBoard(GAMEWINDOWINFO * gi);
	void DrawPiece(char piece, int x, int y);

};