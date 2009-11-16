#include "stdafx.h"
#include "frmGetKey.h"

namespace genkeytool
{
	PieceHashValue _PieceHashValues[PIECE_NUM] = {
		{ 'R', 0, 0, 9 },
		{ 'N', 0, 1, 9 },
		{ 'B', 0, 2, 9 },
		{ 'A', 0, 3, 9 },
		{ 'K', 0, 4, 9 },
		{ 'C', 0, 1, 7 },
		{ 'P', 0, 0, 6 },
		{ 'r', 0, 0, 0 },
		{ 'n', 0, 1, 0 },
		{ 'b', 0, 2, 0 },
		{ 'a', 0, 3, 0 },
		{ 'k', 0, 4, 0 },
		{ 'c', 0, 1, 2 },
		{ 'p', 0, 0, 3 }
	};


	TCHAR * GetFileName(TCHAR * filepath)
	{
		const TCHAR * delimiters = "/\\";

		TCHAR * p;
		TCHAR * plast;
		p = _tcstok (filepath,delimiters); 
		while(p!=NULL) 
		{
			plast = p;
			p = _tcstok(NULL,delimiters); 
		}
		return plast;
	}


	BOOL CALLBACK frmGetKey_EnumChildProc(HWND hwnd,  LPARAM lParam)
	{
		std::string * pStr = (std::string*)lParam;

		TCHAR szBuf[1024];
		TCHAR szStr[256];

		// Get module file name
		GetWindowModuleFileName(hwnd, szStr, sizeof(szStr));

		_stprintf(szBuf,"%s,0x%x",GetFileName(szStr), GetDlgCtrlID(hwnd));

		pStr->append(szBuf);

		return TRUE;
	}

}