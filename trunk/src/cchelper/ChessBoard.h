#pragma once

#include "IGameWindow.h"

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
	static CFastDIB		*m_pMoveRectDIB;
	static PieceStruct	m_tPieceStructs[PIECE_NUM];

public:
	static BOOL LoadMedia();
	static void ReleaseMedia();

private:

	char m_fen[256];

private:
	void DrawPiece( PieceStruct& ps, int x , int y );

public:
	CChessBoard();
	~CChessBoard();

	const char * CatureToFen(IGameWindow * pgw);

	const char * GetFen() { return m_fen; }
	
	void ShowBestMove(int fx, int fy, int tx, int ty);

	void DrawBoard(GAMEWINDOWINFO * gi);
	void DrawPiece(char piece, int x, int y);

};