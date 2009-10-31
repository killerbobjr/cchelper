#pragma once

#define TURN_NONE	0
#define TURN_BLACK	'b'
#define TURN_WHITE	'w'

class IGameWindow
{
public:
	virtual HWND	GetHandle()	= 0;
	virtual DWORD	GetHashValue( int x, int y ) = 0;
	virtual char	GetPiece( int x, int y) = 0;
	virtual int		GetTurn() = 0;

};
