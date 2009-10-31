#pragma once

#include "IGameWindow.h"

// DEFINES
//___________________________________________________________________________

#define MEDIAFILE(f)	_T("../../media/"##f)

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

class CChessBoard
{
	static CFastDIB		*m_pBoardDIB;
	static PieceStruct	m_tPieceStructs[PIECE_NUM];

public:
	static BOOL LoadMedia();
	static void ReleaseMedia();

private:
	void DrawPiece( PieceStruct& ps, int x , int y );

public:
	CChessBoard();
	~CChessBoard();
	
	void DrawBoard(const char * fen);
	void DrawBoard( IGameWindow * pgw );
	void DrawPiece(char piece, int x, int y);
};

// DECLARATIONS OF FUNCTIONS
//___________________________________________________________________________
BOOL InitApp();
BOOL AppLoop();
BOOL ExitApp();