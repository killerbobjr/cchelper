
#include <D3D9.h>
#include <D3dx9tex.h>
#include <stdio.h>
#include <string>
#include <tchar.h>
#include "MurmurHash.h"
#include "capture.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


HRESULT ScreenCapture::CaptureByD3D(HWND hWnd, LPCSTR szFileName)
{
	char chBuf[300];
	HRESULT hr;
	LPDIRECT3D9 pD3D = NULL;
	D3DPRESENT_PARAMETERS D3DPP = {0};
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	D3DDISPLAYMODE D3DMode;
	LPDIRECT3DSURFACE9 pSurface = NULL;

	lstrcpy(chBuf, "Done!");

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
	{
		lstrcpy(chBuf, "Direct3DCreate9() failed.");
		goto Cleanup;
	}

	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3D9::CreateDevice() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

	hr = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &D3DMode);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3D9::GetAdapterDisplayMode() failed with 0x%x.\n", hr);
		goto Cleanup;
	}


	this->m_dwDisplayModeFormat  = D3DMode.Format;

	hr = pD3DDevice->CreateOffscreenPlainSurface(D3DMode.Width,
		D3DMode.Height,
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3DDevice9::CreateOffscreenPlainSurface() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

	hr = pD3DDevice->GetFrontBufferData(0, pSurface);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3DDevice9::GetFrontBufferData() failed with	0x%x.\n",
			hr);
		goto Cleanup;
	}

	RECT rc;

	GetWindowRect(hWnd, &rc);
	hr = D3DXSaveSurfaceToFile(szFileName, D3DXIFF_BMP, pSurface, NULL, &rc);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "D3DXSaveSurfaceToFile() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

Cleanup:
	if (pSurface) pSurface->Release();
	if (pD3DDevice) pD3DDevice->Release();
	if (pD3D) pD3D->Release();
	OutputDebugString(chBuf);

	return hr;
}


HRESULT ScreenCapture::CaptureByD3D(HWND hWnd, LPBYTE *ppData)
{
	char chBuf[300];
	HRESULT hr;
	LPDIRECT3D9 pD3D = NULL;
	D3DPRESENT_PARAMETERS D3DPP = {0};
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	D3DDISPLAYMODE D3DMode;
	LPDIRECT3DSURFACE9 pSurface = NULL;


	*ppData = NULL;

	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
	{
		lstrcpy(chBuf, "Direct3DCreate9() failed.");
		goto Cleanup;
	}

	D3DPP.Windowed = TRUE;
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING, &D3DPP, &pD3DDevice);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3D9::CreateDevice() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

	hr = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &D3DMode);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3D9::GetAdapterDisplayMode() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

	this->m_dwDisplayModeFormat = D3DMode.Format;


	hr = pD3DDevice->CreateOffscreenPlainSurface(D3DMode.Width,
		D3DMode.Height,
		D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pSurface, NULL);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3DDevice9::CreateOffscreenPlainSurface() failed with 0x%x.\n", hr);
		goto Cleanup;
	}

	hr = pD3DDevice->GetFrontBufferData(0, pSurface);
	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3DDevice9::GetFrontBufferData() failed with	0x%x.\n",
			hr);
		goto Cleanup;
	}

	RECT rc;

	GetWindowRect(hWnd, &rc);

	LPD3DXBUFFER pDestBuf;
	hr = D3DXSaveSurfaceToFileInMemory(&pDestBuf, D3DXIFF_BMP, pSurface, NULL, &rc);

	if (FAILED(hr))
	{
		wsprintf(chBuf, "IDirect3DDevice9::D3DXSaveSurfaceToFileInMemory() failed with	0x%x.\n",
			hr);
		goto Cleanup;
	}

	int size = pDestBuf->GetBufferSize();
	LPBYTE pData = (LPBYTE)malloc(size) ;
	memcpy(pData, pDestBuf->GetBufferPointer(), size);
	*ppData = pData;
	pDestBuf->Release();

Cleanup:
	if (pSurface) pSurface->Release();
	if (pD3DDevice) pD3DDevice->Release();
	if (pD3D) pD3D->Release();
	OutputDebugString(chBuf);

	return hr;
}

namespace capture
{

	TCHAR * GetFileName(TCHAR * filepath)
	{
		const TCHAR * delimiters = "/\\";

		TCHAR * p = filepath;
		TCHAR * plast = p;
		p = _tcstok (filepath,delimiters); 
		while(p!=NULL) 
		{
			plast = p;
			p = _tcstok(NULL,delimiters); 
		}
		return plast;
	}


	BOOL CALLBACK _Capture_EnumChildProc(HWND hwnd,  LPARAM lParam)
	{
		std::string * pStr = (std::string*)lParam;

		TCHAR szBuf[1024];
		TCHAR szStr[256];

		szStr[0]=0;
		// Get module file name
		GetWindowModuleFileName(hwnd, szStr, sizeof(szStr));

		_stprintf(szBuf,"%s,0x%x",GetFileName(szStr), GetDlgCtrlID(hwnd));

		pStr->append(szBuf);

		return TRUE;
	}


	unsigned int GetWindowKey(HWND hwnd)
	{
		unsigned int key;

		std::string strHash;

		EnumChildWindows(hwnd, _Capture_EnumChildProc, (LPARAM) &strHash);

		OutputDebugString(strHash.c_str());
		key = base::MurmurHash2(strHash.c_str(), strHash.length());

		return key;
	}

}