#include "common.h"
#include "fastdib.h"
#include "app.h"
#include "IGameWindow.h"
#include "ChessEngine.h"
#include "ChessBoard.h"
#include "AppEnv.h"
#include "resource.h"
#include <fstream>
#include "Json/Json.h"

using namespace base;

// CChessBoard
//___________________________________________________________________________

CChessBoard::CChessBoard()
{
	m_pGameWindow = NULL;
	m_pChessEngine = NULL;

	m_pBoardDIB	= NULL;
	m_pBackgroundDIB	= NULL;
	m_pMoveRectDIB	= NULL;
	memset(&m_tPieceStructs, 0, sizeof(PieceStruct) * PIECE_NUM);
}

CChessBoard::~CChessBoard()
{
	ReleaseMedia();
}

void CChessBoard::GetBoardSize(SIZE * size)
{
	assert(CChessBoard::m_pBoardDIB);

	size->cx = CChessBoard::m_pBoardDIB->GetDibInfo()->m_bmWidth ;
	size->cy = CChessBoard::m_pBoardDIB->GetDibInfo()->m_bmHeight;
}

BOOL CChessBoard::LoadMedia()
{
	try
	{
		int i;
		HRESULT hr;

		Json::Value  vRoot;

		std::ifstream fs(AppEnv::GetMediaPath(_T("theme.ini"), AppEnv::szTheme));
		if(fs)
		{
			Json::Reader reader;
			reader.parse(fs, vRoot, false);

			m_ptBoardOrigin.x = vRoot["BoardOringinX"].asInt();
			m_ptBoardOrigin.y = vRoot["BoardOringinY"].asInt();
			m_sizePiece.cx = vRoot["PieceWidth"].asUInt();
			m_sizePiece.cy = vRoot["PieceHeight"].asUInt();
			m_sizeSquare.cx = vRoot["SquareWidth"].asUInt();
			m_sizeSquare.cy = vRoot["SquareHeight"].asUInt();

			if ( !m_pBoardDIB )
			{
				m_pBoardDIB = new CFastDIB();
				if ( m_pBoardDIB )
				{
					hr = m_pBoardDIB->LoadFromFile(
						AppEnv::GetMediaPath(vRoot["BoardBmp"].asString().c_str(),
						AppEnv::szTheme)
						);
					THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
				}
			}

			if ( !m_pBackgroundDIB )
			{
				m_pBackgroundDIB = new CFastDIB();
				if( m_pBackgroundDIB )
				{
					hr = m_pBackgroundDIB->LoadFromFile(
						AppEnv::GetMediaPath(vRoot["BackGroundBmp"].asString().c_str(),
						AppEnv::szTheme)
						);
					THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
				}
			}

			if( !m_pMoveRectDIB )
			{
				m_pMoveRectDIB = new CFastDIB();
				if( m_pMoveRectDIB )
				{
					hr = m_pMoveRectDIB->LoadFromFile(
						AppEnv::GetMediaPath(vRoot["MoveRectBmp"].asString().c_str(),
						AppEnv::szTheme)
						);
					THROW_CHECK(hr, CFastDIB::GetErrorString(hr));				
				}
			}

			Json::Value vPieceBmp = vRoot["PieceBmps"] ;

			for ( i = 0; i < PIECE_NUM; i++ )
			{
				if ( !m_tPieceStructs[i].pDib )
				{
					m_tPieceStructs[i].pDib = new CFastDIB();
					m_tPieceStructs[i].cPiece = vPieceBmp[i]["PieceKey"].asInt();

					hr = m_tPieceStructs[i].pDib->LoadFromFile(
						AppEnv::GetMediaPath(vPieceBmp[i]["PieceBmp"].asString().c_str(),
						AppEnv::szTheme)
						);
					THROW_CHECK(hr, CFastDIB::GetErrorString(hr));
				}
			}

		}
		fs.close();
		
		return true;
	}
	catch(std::exception& e)
	{
		base::Log(0, e.what() );
		return false;
	}
}


void CChessBoard::ReleaseMedia()
{
	int i;
	for ( i = 0; i < PIECE_NUM; i++ )
	{
		if ( m_tPieceStructs[i].pDib )
		{
			delete m_tPieceStructs[i].pDib ;
			m_tPieceStructs[i].pDib = NULL;
		}
	}

	if ( m_pBoardDIB )
	{
		delete m_pBoardDIB ;
		m_pBoardDIB = NULL;
	}

	if ( m_pBackgroundDIB  )
	{
		delete m_pBackgroundDIB;
		m_pBackgroundDIB = NULL;
	}

	if( m_pMoveRectDIB )
	{
		delete m_pMoveRectDIB;
		m_pMoveRectDIB = NULL;
	}

}

void CChessBoard::DrawPiece( PieceStruct& ps, int squarex , int squarey )
{
	POINT pt = GetSquareOrigin(squarex, squarey);

	ps.pDib->Draw( g_pMainSurface, DRAWMODE_NORMAL, pt.x, pt.y );
}

void CChessBoard::DrawPiece(char piece, int x, int y)
{
	for(int i=0; i < PIECE_NUM; i++){
		if( CChessBoard::m_tPieceStructs[i].cPiece == piece)
		{
			DrawPiece(CChessBoard::m_tPieceStructs[i], x, y);
			break;
		}
	}
}

POINT CChessBoard::GetSquareOrigin(int squarex, int squarey )
{
	POINT pt;
	pt.x = m_ptBoardOrigin.x + squarex * m_sizeSquare.cx;
	pt.y = m_ptBoardOrigin.y + squarey * m_sizeSquare.cy;
	return pt;
}

void CChessBoard::ShowBestMove(CChessEngine::PieceMove * mv)
{
	assert(mv);
	assert(m_pGameWindow);


	GAMEWINDOWINFO gwi = m_pGameWindow->GetGameWindowInfo();

	if( gwi.bAvailible && !gwi.bGameOver && gwi.PlayerColor == gwi.Turn )
	{
		POINT ptSrc, ptDst;

		if( gwi.PlayerColor == TURN_WHITE )
		{
			ptSrc = this->GetSquareOrigin(mv->from.x, mv->from.y);
			ptDst = this->GetSquareOrigin(mv->to.x , mv->to.y );
		}
		else if( gwi.PlayerColor == TURN_BLACK )
		{
			ptSrc = this->GetSquareOrigin(mv->from.x, 9 - mv->from.y );
			ptDst = this->GetSquareOrigin(mv->to.x, 9 - mv->to.y );
		}

		static BOOL bShowSrcFocus = TRUE;
		
		bShowSrcFocus = !bShowSrcFocus;
		if( bShowSrcFocus )
		{
			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, ptSrc.x , ptSrc.y);
		} 
		else
		{
			m_pMoveRectDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL, ptDst.x, ptDst.y);
		}
	}
	
}

void CChessBoard::Update()
{
	assert( GetGameWindow() && GetChessEngine() );

	GetChessEngine()->UpdateState();

	GAMEWINDOWINFO gi ;
	WINDOWPLACEMENT wp;
	GetWindowPlacement( GetGameWindow()->GetFrameWindowHandle(),&wp);

	if (wp.showCmd == SW_SHOWNORMAL)
	{
		memset( &gi, 0, sizeof(gi));
		if( GetGameWindow()->ReadGameWindowInfo() )
		{
			gi = GetGameWindow()->GetGameWindowInfo();
			if( gi.bAvailible && !gi.bGameOver )
			{
				DrawBoard( &gi );

				if( gi.bObserverMode )
					EnableMenuItem(GetMenu(g_hWndMain),IDM_AUTOPLAY,MF_DISABLED);
				else
					EnableMenuItem(GetMenu(g_hWndMain),IDM_AUTOPLAY,MF_ENABLED);

				if( gi.PlayerColor != gi.Turn )
				{
					KillAlarm();
					GetChessEngine()->Stop();
					SetWindowText(g_hWndMain, _T("waiting..."));
				}
				else 
				{
					if( GetChessEngine()->GetState() == IChessEngine::Idle )
					{
						//it is player turn, and engine idle.
						IChessEngine::PieceMove * mv;
						mv = GetChessEngine()->GetBestMove();
						if ( !mv )
						{
							//there are no best move and engine idle, make a think...
							GetChessEngine()->Go(gi.szFen);
						}
						else 
						{
							//already have a bestmove then, make the move.
							SetWindowText(g_hWndMain, _T("Got a move"));
							ShowBestMove(mv);
							if( AppEnv::bAutoPlay && !gi.bObserverMode )
							{
								if ( mv->mvtimestamp == 0 )
								{
									mv->mvtimestamp = time(NULL);
									GetGameWindow()->MovePiece(mv);
								} 
								else
								{
									time_t now = time(NULL);
									if( now - mv->mvtimestamp > 1 )
									{
										SetAlarm();
									}
								}
							}
						}
					} 
					else 
					{
						// todo : draw thinking....
						SetWindowText(g_hWndMain, _T("thinking..."));
					}
				}
			}
		}
	} 
	else if ( wp.showCmd == SW_SHOWMINIMIZED )
	{
		//DrawBoard( NULL );
	}

}


void CChessBoard::DrawBoard(char * fen)
{
	assert( g_pMainSurface && m_pBoardDIB )	;

	m_pBackgroundDIB->Draw(g_pMainSurface);
	m_pBoardDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL);


	char *lpFen = fen ;
	int x = 0;
	int y = 0;
	char c;

	c = *lpFen;

	while( c && c != ' ')
	{
		if( c >= '0' && c <= '9' )
		{
			x = x + c - '0';
		}
		else if( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') 
		{
			DrawPiece( c , x, y );
			x = x + 1;
		}else if( c == '/' )
		{
			x = 0;
			y ++;
		}

		c= *(++lpFen);
	}
}

void CChessBoard::DrawBoard(GAMEWINDOWINFO * gi)
{
	assert( g_pMainSurface && m_pBoardDIB )	;

	m_pBackgroundDIB->Draw(g_pMainSurface);
	m_pBoardDIB->Draw( g_pMainSurface, DRAWMODE_NORMAL);

	if ( gi == NULL )
	{
		return;
	}

	char *lpFen = gi->szFen ;
	int x = 0;
	int y = 0;
	char c;

	c = *lpFen;

	while( c && c != ' ')
	{
		if( c >= '0' && c <= '9' )
		{
			x = x + c - '0';
		}
		else if( c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') 
		{
			if( gi->PlayerColor == TURN_WHITE )
				DrawPiece( c , x, y );
			else if( gi->PlayerColor == TURN_BLACK)
				DrawPiece( c, x, 9 - y);
			x = x + 1;
		}else if( c == '/' )
		{
			x = 0;
			y ++;
		}

		c= *(++lpFen);
	}
}


/*
void CChessBoard::DrawBoard( IGameWindow * pgw )
{
	assert(pgw && g_pMainSurface && m_pBoardDIB);

	if( !pgw->GetHandle() )
	{
		return ;
	}

	int p;
	int x;
	int y;

	g_pMainSurface->ClearImage();
	m_pBoardDIB->Draw( g_pMainSurface );

	for( y =0; y < 10; y++ )
	{
		for( x = 0; x < 9; x ++)
		{
			p = pgw->GetPiece(x,y);
			if ( p )	this->DrawPiece(p, x, y);
		}
	}
	
	int turn = pgw->GetTurn();
	if( turn == 'b' )
	{
		this->DrawPiece( 'k' , 9, 1 );
	} else if ( turn == 'w' )
	{
		this->DrawPiece( 'K' , 9, 1 );
	}
}
*/