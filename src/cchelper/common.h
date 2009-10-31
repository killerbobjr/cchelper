#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
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

#include "fastdib.h"
// DEFINES
//___________________________________________________________________________

#define MAX_LOADSTRING			256
#define APPUPDATE_TIMER			1
#define APPUPDATE_TIME_ELAPSE	500
// Global variables
//___________________________________________________________________________

extern POINT		g_ptMouse;
extern int			g_intAppRunning;
extern BOOL			g_bActive;
extern HWND			g_hWndMain;
extern BOOL			g_bInitialized;
extern CFastDIB		*g_pMainSurface;

// FUNCTIONS
//___________________________________________________________________________
namespace base
{
	void Log(int nBugFileID,const char * format,...);
}