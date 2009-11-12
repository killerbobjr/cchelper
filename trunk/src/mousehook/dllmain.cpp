// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "mousehook.h"
#include "log.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CMouseHook::m_hModule = hModule;
		base::Log(9,"DLL_PROCESS_ATTACH",CMouseHook::m_hWnd);
		return TRUE;
	case DLL_THREAD_ATTACH:
		base::Log(9,"DLL_THREAD_ATTACH,%x",CMouseHook::m_hWnd );
		break;
	case DLL_THREAD_DETACH:
		base::Log(9,"DLL_THREAD_DETACH,%x",CMouseHook::m_hWnd);
		break;
	case DLL_PROCESS_DETACH:
		base::Log(9,"DLL_PROCESS_DETACH,%x",CMouseHook::m_hWnd);
		break;
	}
	return TRUE;
}

