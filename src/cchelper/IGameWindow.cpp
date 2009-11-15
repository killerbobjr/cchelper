#include "common.h"
#include "IGameWindow.h"

int IGameWindow::nGameWindowNum = 0;
IGameWindow * IGameWindow::pGameWindows[GAMEWINDOW_MAXLEN];

void IGameWindow::AddGameWindow(IGameWindow * p){
	pGameWindows[nGameWindowNum++] = p;
}

IGameWindow * IGameWindow::SearchGameWindow() {
	for( int i = 0; i< nGameWindowNum; i++)
	{
		if( pGameWindows[i] && pGameWindows[i]->FindGameWindow() )
		{
			return pGameWindows[i];
		}
	}
	return NULL;
}
void IGameWindow::ReleaseAllGameWindow()
{
	for( int i=0; i< nGameWindowNum ; i++)
	{
		if( pGameWindows[i]) delete pGameWindows[i];
	}
}
