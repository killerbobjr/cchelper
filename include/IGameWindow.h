#pragma once

#include "IChessEngine.h"

#define FEN_MAXLENGHT	(100)

#define TURN_NONE	0
#define TURN_BLACK	'b'
#define TURN_WHITE	'w'

// @description
// Structure that contain game window information.
//________________________________________________________________________________________
struct GAMEWINDOWINFO
{
	bool bAvailible;			// If the ReadGameWindowInfo() succeeds.
	char szFen[FEN_MAXLENGHT];	// chess board fen string.
	int	 PlayerColor;			// the player color. 0:Observer, 'b':Black, 'w':White
	int  Turn;					// current turn
	bool bObserverMode;			// observer mode flag
	bool bTurnChanged;			// If the trun changed, the flag will be set.
	bool bGameOver;				// the game over flag 
};

// @description
// Interface of game window.
// Find and maintain the handle of the actual game window. 
//________________________________________________________________________________________
class IGameWindow
{

	virtual char	GetPiece( int x, int y) = 0;

protected:

	IChessEngine * m_pChessEngine;			//Point of chess engine.
	GAMEWINDOWINFO	m_GameWindowInfo;		//Contain the game window information.

	IGameWindow()
	{
		m_pChessEngine = 0;
		memset(&m_GameWindowInfo, 0, sizeof(m_GameWindowInfo));
	}

public:

	void SetChessEngine(IChessEngine* pce)
	{
		if( pce != m_pChessEngine )
			m_pChessEngine = pce;
	}

	IChessEngine* GetChessEngine()
	{
		return m_pChessEngine;
	}

	// @description
	// Get game window information that ReadGameWindowInfo() read.
	// @return
	// GAMEWINDOWINFO that ReadGameWindowInfo() read.
	// @remark
	// Befor use it, please call ReadGameWindowInfo() first.
	// And check GAMEWINDOWINFO.bAvailible flag first.
	//_________________________________________________________________________________________
	const GAMEWINDOWINFO& GetGameWindowInfo()
	{
		return m_GameWindowInfo;
	}

public:
	// @description
	//	Read game information from the game window.
	//_________________________________________________________________________________________
	virtual bool	ReadGameWindowInfo() = 0;


	// @description
	// Get handle of the actual game window
	// @return
	// If the fucntion succeeds, the return value is the handle of the actual game window.
	// If no game window or the game window is unavailable, the return value is NULL.
	//_________________________________________________________________________________________
	virtual HWND	GetHandle()	= 0;


	// @description
	// Move piece from specified location to another specified location.
	// @para int fx:
	// @return
	// If the move is done, the return value TRUE, otherwise return FALSE.
	//_________________________________________________________________________________________
	virtual BOOL MovePiece(IChessEngine::PieceMove * pmv) = 0;


	// @description
	// Get frame window 
	//_________________________________________________________________________________________
	virtual HWND GetFrameWindowHandle() = 0;

	virtual bool SaveHashValue(TCHAR * szFile, TCHAR * szBmp){ return true;};
};
