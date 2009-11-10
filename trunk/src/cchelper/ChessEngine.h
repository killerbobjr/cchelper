#pragma once

#include "IChessEngine.h"
#include <time.h>

#define INPUT_BUFFER_SIZE		(4096)
struct PipeStruct;

class CChessEngine
	:public IChessEngine
{


private:
	PipeStruct * m_pPipe;
	bool m_bLoaded;
	char m_szInputBuf[INPUT_BUFFER_SIZE];

	EngineState m_state;

	bool m_bHasBestMove;
	PieceMove m_mvBestMove;
	int m_nSkipBestMoveCount;

public:
	CChessEngine(void);
	virtual ~CChessEngine(void);


	EngineState GetState() { return m_state; }

	PieceMove * GetBestMove() { if ( m_bHasBestMove ) return &m_mvBestMove ; else return NULL ; };

	virtual void UpdateState();

	virtual void Go(char * szFen) ;
	void Stop();
	
	void SendCommand(const char * cmd);

	BOOL IsLoaded(){ return m_bLoaded;}
	BOOL InitEngine(TCHAR * szEngineFile);

	time_t GetBestMoveElapse();
	virtual void Restart();
	virtual void GameOver();
};
