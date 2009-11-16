#include "StdAfx.h"
#include "frmMain.h"
#include "D3D9.h"
#include "D3dx9tex.h"

namespace genkeytool {

	extern "C" {
		void Capture3D(HWND hWnd, LPCSTR szFileName)
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
		}
	}
}