#pragma once

class IChessEngine
{
public:
	virtual void Restart() = 0;
	virtual void GameOver() = 0;
};
