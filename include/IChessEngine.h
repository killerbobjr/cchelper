#pragma once



class IChessEngine
{
public:

	struct PieceMove
	{
		POINT from;			// position of the source location
		POINT to;			// position of the target location
		time_t timestamp;	// timestamp of getting the best move
	};

	enum EngineState
	{
		BusyWait,
		Idle,
	};

public:
	virtual void Restart() = 0;
	virtual void GameOver() = 0;
};
