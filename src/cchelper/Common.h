#pragma once

// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>

//#define ENGINE_CCE

#include "log.h"
#include "fastdib.h"
#include "BitmapEx.h"
#include "ChessEngine.h"
// DEFINES
//___________________________________________________________________________

#define MAX_LOADSTRING			256
#define APPUPDATE_TIMER			1
#define APPUPDATE_TIMER_ELAPSE	250


// Global variables
//___________________________________________________________________________

extern POINT		g_ptMouse;
extern int			g_intAppRunning;
extern BOOL			g_bActive;
extern HWND			g_hWndMain;
extern BOOL			g_bInitialized;
extern CFastDIB		*g_pMainSurface;
extern CBitmapEx	*g_pMainSurfaceBitmapEx;
extern CChessEngine	* g_pChessEngine;
// FUNCTIONS
//___________________________________________________________________________
