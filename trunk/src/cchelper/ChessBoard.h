#pragma once

#include "IGameWindow.h"
#include "IChessEngine.h"

// DEFINES
//___________________________________________________________________________s



#define PIECE_NUM	(14)

#define MAX_LOADSTRING	256

#define THROW_CHECK(hr,s) if (hr != S_OK) throw(s);

// STRUCTS
//___________________________________________________________________________

struct PieceStruct
{
	char		cPiece;
	CFastDIB	* pDib;
};


// @description
// Draw helper chess board.
class CChessBoard
{
	CFastDIB		*m_pBoardDIB;
	CFastDIB		*m_pBackgroundDIB;
	CFastDIB		*m_pMoveRectDIB;
	PieceStruct	m_tPieceStructs[PIECE_NUM];

	POINT	m_ptBoardOrigin;
	SIZE	m_sizePiece;
	SIZE	m_sizeSquare;

public:
	BOOL LoadMedia();
	void ReleaseMedia();
	void GetBoardSize(SIZE * size);


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
		if( GetChessEngine() )
		{
			GetChessEngine()->Restart();
		}
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
	void DrawBoard(char * fen);

};