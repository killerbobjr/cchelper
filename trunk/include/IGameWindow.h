#pragma once

#define TURN_NONE	0
#define TURN_BLACK	'b'
#define TURN_WHITE	'w'

struct GAMEINFO
{
	char szFen[100];
	int	 PlayerColor;
	int  Turn;
};

class IGameWindow
{
	virtual char	GetPiece( int x, int y) = 0;
public:
	virtual bool	ReadWindow(GAMEINFO * pgi) = 0;
	virtual HWND	GetHandle()	= 0;
};
