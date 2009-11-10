#pragma once



class IChessEngine
{
public:

	struct PieceMove
	{
		POINT from;			// position of the source location
		POINT to;			// position of the target location
		time_t mvtimestamp;	// timestamp of getting the best move
	};

	enum EngineState
	{
		BusyWait,
		Idle,
	};

public:
	virtual EngineState GetState() = 0;
	virtual BOOL IsLoaded() = 0;
	virtual void UpdateState() = 0;
	virtual void Restart() = 0;
	virtual void GameOver() = 0;

	virtual void Go(char * szFen) = 0;
	virtual void Stop() = 0;
	virtual PieceMove * GetBestMove() = 0 ;
};
