#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "log.h"
#include "appenv.h"

using namespace base;

TCHAR AppEnv::szAppPath[_MAX_PATH];
TCHAR AppEnv::szMediaPath[_MAX_PATH];
TCHAR AppEnv::szEngine[_MAX_PATH]; 
TCHAR AppEnv::szTheme[_MAX_PATH];

BOOL AppEnv::bAutoPlay = FALSE;
int	AppEnv::nThinkTime ;

bool AppEnv::LoadEnv(TCHAR * szSettingFile)
{
	_tgetcwd(AppEnv::szAppPath, sizeof(AppEnv::szAppPath));
	GetPrivateProfileString(
		_T("AppEnv"), 
		_T("MediaPath"), 
		_T("../../media/"), 
		AppEnv::szMediaPath, 
		sizeof(AppEnv::szMediaPath), 
		szSettingFile
		);
	GetPrivateProfileString(
		_T("AppEnv"), 
		_T("ChessEngine"), 
		_T("eleeye.exe"), 
		AppEnv::szEngine, 
		sizeof(AppEnv::szEngine), 
		szSettingFile
		);

	GetPrivateProfileString(
		_T("AppEnv"), 
		_T("theme"), 
		_T("default"), 
		AppEnv::szTheme, 
		sizeof(AppEnv::szTheme), 
		szSettingFile
		);

	AppEnv::nThinkTime = GetPrivateProfileInt(_T("AppEnv"), _T("ThinkTime"),10,szSettingFile);

	return true;
}

TCHAR * AppEnv::GetMediaPath(const TCHAR * szMediaFile, const TCHAR * szTheme)
{
	static TCHAR szBuf[_MAX_PATH];

	if( !szTheme )
		_sntprintf(szBuf, sizeof(szBuf), _T("%s%s"), AppEnv::szMediaPath, szMediaFile);
	else
		_sntprintf(szBuf, sizeof(szBuf), _T("%s\\theme\\%s\\%s"), AppEnv::szMediaPath, szTheme, szMediaFile);

	return szBuf;
}

