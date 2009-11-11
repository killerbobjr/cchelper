#pragma once

#include <windows.h>

extern "C" {
void DoPageAction(HWND hwnd, DWORD action);

long DisplayHTMLStr(HWND hwnd, LPCTSTR string);
long DisplayHTMLPage(HWND hwnd, LPTSTR webPageName);
void ResizeBrowser(HWND hwnd,int left, int top, DWORD width, DWORD height);
long EmbedBrowserObject(HWND hwnd);

void UnEmbedBrowserObject(HWND hwnd);

}