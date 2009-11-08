#pragma once

#include "IChessEngine.h"

#define TURN_NONE	0
#define TURN_BLACK	'b'
#define TURN_WHITE	'w'

struct GAMEINFO
{
	char szFen[100];
	int	 PlayerColor;
	int  Turn;
	bool bObserverMode;
};

class IGameWindow
{

	virtual char	GetPiece( int x, int y) = 0;
protected:
	IChessEngine * m_pChessEngine;
public:
	virtual bool	ReadWindow(GAMEINFO * pgi) = 0;
	virtual HWND	GetHandle()	= 0;

	virtual BOOL MovePiece(int fx, int fy, int tx, int ty ) = 0;

	void SetChessEngine(IChessEngine* pce)
	{
		m_pChessEngine = pce;
	}

};
