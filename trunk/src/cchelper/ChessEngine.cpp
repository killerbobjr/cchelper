#include "common.h"
#include "ChessEngine.h"
#include "pipe.h"
#include <time.h>

CChessEngine::CChessEngine(void)
{
	m_pPipe = NULL;
	m_bLoaded = false;
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
	if ( m_pPipe )
	{
		if(m_pPipe->LineInput(m_szInputBuf))
		{
			if( strncmp(m_szInputBuf, "bestmove ", 9) == 0 )
			{
				base::Log(2,m_szInputBuf);
				this->m_state = CChessEngine::Idle ;
				this->m_bHasBestMove = true;
				this->m_mvBestMove.fx = m_szInputBuf[9] - 'a';
				this->m_mvBestMove.fy = 9 - (m_szInputBuf[10] - '0');
				this->m_mvBestMove.tx = m_szInputBuf[11] - 'a';
				this->m_mvBestMove.ty = 9 - (m_szInputBuf[12] - '0');
			}
		}
	}
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
}

void CChessEngine::Stop()
{
	if ( m_pPipe )
	{
		SendCommand("stop");
		while(m_pPipe->LineInput(m_szInputBuf))
		{
			if( strncmp(m_szInputBuf, "bestmove ", 9) == 0 )
				break;
		}
		this->m_state = CChessEngine::Idle ;
		this->m_bHasBestMove = false;
	}
}

void CChessEngine::SendCommand(const char * cmd)
{
	if( m_pPipe )
	{
		if( strncmp(cmd, "go", 2) == 0 )
		{
			this->m_bHasBestMove = false;
			this->m_state = CChessEngine::BusyWait ;
		}

		m_pPipe->LineOutput(cmd);
		base::Log(2,cmd);
	}
}

bool CChessEngine::InitEngine(TCHAR * szEngineFile)
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

