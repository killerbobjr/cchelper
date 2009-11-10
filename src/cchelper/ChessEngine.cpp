#include "common.h"
#include "ChessEngine.h"
#include "pipe.h"
#include "appenv.h"
#include <time.h>

CChessEngine::CChessEngine(void)
{
	m_pPipe = NULL;
	m_bLoaded = false;
	m_nSkipBestMoveCount = 0;
}



CChessEngine::~CChessEngine(void)
{
	if ( m_pPipe ) 
	{
		if ( IsLoaded())
			m_pPipe->LineOutput("quit");
		m_pPipe->Close();
		delete m_pPipe;
	}
}

void CChessEngine::UpdateState()
{
	static char last_bestmove[100];

	if ( m_pPipe )
	{
		if(m_pPipe->LineInput(m_szInputBuf))
		{
			if( strncmp(m_szInputBuf, "bestmove ", 9) == 0 )
			{
				if( strcmp(m_szInputBuf, last_bestmove) == 0 )
				{
					base::Log(2, "%s the same best move", m_szInputBuf);
				}
				if ( m_nSkipBestMoveCount > 0 )
				{
					base::Log(2,"%s skip",m_szInputBuf);
					m_nSkipBestMoveCount --;
				}
				else
				{
					base::Log(2,m_szInputBuf);
					this->m_state = CChessEngine::Idle ;
					this->m_bHasBestMove = true;
					this->m_mvBestMove.from.x  = m_szInputBuf[9] - 'a';
					this->m_mvBestMove.from.y  = 9 - (m_szInputBuf[10] - '0');
					this->m_mvBestMove.to.x = m_szInputBuf[11] - 'a';
					this->m_mvBestMove.to.y = 9 - (m_szInputBuf[12] - '0');
					this->m_mvBestMove.timestamp = time(NULL);
					this->m_BestMoveTime = time(NULL);

					if( AppEnv::bAutoPlay && m_pGameWindow )
					{
						m_pGameWindow->MovePiece(&m_mvBestMove);
					}
				}
			}
		}
	}
}

void CChessEngine::GameOver()
{
	if( !IsLoaded() )
		return;

	if ( this->GetState() == CChessEngine::BusyWait )
	{
		this->Stop();
	}

	this->m_bHasBestMove = false;
	base::Log(0,"GameOver");
}

void CChessEngine::Restart()
{
	if( !IsLoaded() )
		return;

	if ( this->GetState() == CChessEngine::BusyWait )
	{
		this->Stop();
	}

	this->m_bHasBestMove = false;
	base::Log(0,"Restart");
}

void CChessEngine::Stop()
{
	if ( m_pPipe )
	{
		SendCommand("stop");
		
		if( g_pChessEngine->GetState() == CChessEngine::BusyWait )
		{
			this->m_nSkipBestMoveCount ++;
		}
		
		this->m_state = CChessEngine::Idle ;
		this->m_bHasBestMove = false;
	}
}

//@description
//If turn changed and engine still thinking(player make a move before 
// engine drag out a move), then stop it and make an new think.
//_________________________________________________________________________________
void CChessEngine::Go(char * szFen)
{
	char szCmd[1024];
	assert(szFen);

	if( IsLoaded() )
	{
		if( GetState() == IChessEngine::BusyWait)
		{
			Stop();
		}
		sprintf(szCmd, "position fen %s", szFen );
		SendCommand(szCmd);
		sprintf(szCmd, "go time %d", AppEnv::nThinkTime );
		SendCommand(szCmd);
		m_bHasBestMove = false;
		m_state = IChessEngine::BusyWait ;

	}
}

void CChessEngine::SendCommand(const char * cmd)
{
	if( m_pPipe )
	{
		m_pPipe->LineOutput(cmd);
		base::Log(2,cmd);
	}
}

time_t CChessEngine::GetBestMoveElapse()
{
	if( m_bHasBestMove )
	{
		time_t now;
		now = time(NULL);
		return( now - this->m_BestMoveTime );
	} 
	else
	{
		return 0;
	}
}

BOOL CChessEngine::InitEngine(TCHAR * szEngineFile)
{
	if( !m_pPipe )
		m_pPipe = new PipeStruct();

	m_pPipe->Open(szEngineFile);

	m_pPipe->LineOutput("ucci");

	time_t starttime;

	starttime = time(NULL);

	m_bLoaded = false;

	while( !m_bLoaded )
	{
		if(m_pPipe->LineInput(m_szInputBuf))
		{
			if( strcmp(m_szInputBuf, "ucciok") == 0 )
			{
				m_bLoaded = true;
				m_state = CChessEngine::Idle ;
			}
		}else
		{
			Sleep(1);
			time_t nowtime;
			nowtime = time(NULL);

			if( nowtime - starttime > 30)
			{
				m_pPipe->Close();
				break;
			}
		}
	}

	m_pPipe->LineOutput("setoption usemillisec false");

	return m_bLoaded;
}

