#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <math.h>
#include "FastDIB.h"


//
//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

#define TCM_EQUAL 0
#define TCM_BELONG 1

#define _SATURATION255(color) if (color<0) color = 0; if (color>255) color = 255;
#ifdef _DEBUG
#define _CHECKTYPE(pDibInfo, uDibType, uTypeMode) 
//{ if (uTypeMode == TCM_BELONG) { if (pDibInfo->m_nType != uDibType) return E_TYPEMISMATCH; } else { if ((pDibInfo->m_nType&uDibType)==0) return E_TYPEMISMATCH; }}

#else
#define _CHECKTYPE(pDibInfo, uDibType, uTypeMode)
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFastDIB::CFastDIB()
{
	ZeroMemory(&m_DibInfo, sizeof FASTDIB);
	ZeroMemory(&m_DibInfoBack, sizeof FASTDIB);
	m_bLocked = FALSE;
}

CFastDIB::~CFastDIB()
{
	if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
	{
		if (m_DibInfo.m_pvData != NULL)
			delete []m_DibInfo.m_pvData;
	}
	if ((m_DibInfo.m_nType&FDIBTYPE_NULLBACK)!=FDIBTYPE_NULLBACK)
	{
		if (m_DibInfoBack.m_pvData != NULL)
			delete []m_DibInfoBack.m_pvData;
	}
}

void CFastDIB::_MatrixTrans(POINT *pcpCoord, FDIBMATRIX fdMatrix)
{
	DWORD tmpx = pcpCoord->x;
	DWORD tmpy = pcpCoord->y;
	pcpCoord->x = tmpx*fdMatrix.aPart[0][0]+tmpy*fdMatrix.aPart[1][0]+fdMatrix.aPart[2][0];
	pcpCoord->y = tmpx*fdMatrix.aPart[0][1]+tmpy*fdMatrix.aPart[1][1]+fdMatrix.aPart[2][1];
}

void CFastDIB::_MatrixTrans(FDIBMATRIX *pfdMatrix1, FDIBMATRIX fdMatrix2)
{
	FDIBMATRIX tmpfdMatrix;
	ZeroMemory(&tmpfdMatrix, sizeof FDIBMATRIX);
	memcpy(&tmpfdMatrix, pfdMatrix1, sizeof FDIBMATRIX);
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
		{
			pfdMatrix1->aPart[i][j] = tmpfdMatrix.aPart[i][0]*fdMatrix2.aPart[0][j]+tmpfdMatrix.aPart[i][1]*fdMatrix2.aPart[1][j]+tmpfdMatrix.aPart[i][2]*fdMatrix2.aPart[2][j];
		}
	}
}

HRESULT CFastDIB::LoadFromFile(LPCTSTR lpszFileName)
{
	if (!PathFileExists(lpszFileName))
		return E_FILENOTFOUND;
	/////////////清除原有信息//////////////
	return LoadFromHBitmap((HBITMAP)LoadImage(NULL, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION));
}

const char * CFastDIB::GetErrorString(HRESULT hErrorCode)
{
	switch (hErrorCode)
	{
	case 0x90000001:	return ("unSuport color mode");
	case 0x90000002:	return ("loadimage fail");
	case 0x90000003:	return ("getobject fail");
	case 0x90000004:	return ("file not found");
	case 0x90000005:	return ("type mismatch");
	case 0x90000006:	return ("invalid size");
	case 0x90000007:	return ("empty object");
	case 0x90000008:	return ("dib locked");
	case 0x90000009:	return ("not inited");
	case E_FAIL:		return ("E_FAIL");
	default:			return ("UnExpected Error");
	}
}

HRESULT CFastDIB::LoadFromDIB(CFastDIB *pSrcDib)
{
	if (pSrcDib == NULL)
		return E_FAIL;

	_CHECKTYPE(pSrcDib->GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	if ((pSrcDib->GetDibInfo()->m_nType==FDIBTYPE_DOUBLEBUF)&&(!CFastDIB::m_bInited))
		return E_NOTINIT;

	if (!(pSrcDib->GetDibInfo()->m_nType==FDIBTYPE_DOUBLEBUF))
	{
		if (m_DibInfo.m_bmHeight*m_DibInfo.m_bmWidth < (pSrcDib->GetDibInfo()->m_bmHeight * pSrcDib->GetDibInfo()->m_bmWidth))
		{
			if (m_DibInfo.m_pvData!=NULL)
				delete[] m_DibInfo.m_pvData;
			m_DibInfo.m_pvData = new DWORD[pSrcDib->GetDibInfo()->m_bmHeight * pSrcDib->GetDibInfo()->m_bmWidth];
		}
		if (pSrcDib->GetBackDibInfo()->m_pvData != NULL)
		{
			delete []pSrcDib->GetBackDibInfo()->m_pvData;
			pSrcDib->GetBackDibInfo()->m_pvData = NULL;
		}
		memcpy(m_DibInfo.m_pvData, pSrcDib->GetDibInfo()->m_pvData, (pSrcDib->GetDibInfo()->m_bmHeight)*(pSrcDib->GetDibInfo()->m_bmWidth)*4);
	}
	else
	{
		if (m_DibInfo.m_pvData == NULL)
			m_DibInfo.m_pvData = new DWORD[CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight];
		if (m_DibInfoBack.m_pvData == NULL)
			m_DibInfoBack.m_pvData = new DWORD[CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight];
		memcpy(m_DibInfo.m_pvData, pSrcDib->GetBackDibInfo()->m_pvData, (pSrcDib->GetDibInfo()->m_bmHeight)*(pSrcDib->GetDibInfo()->m_bmWidth)*4);
		memcpy(m_DibInfoBack.m_pvData, pSrcDib->GetBackDibInfo()->m_pvData, (pSrcDib->GetDibInfo()->m_bmHeight)*(pSrcDib->GetDibInfo()->m_bmWidth)*4);
	}
	m_DibInfo.m_bmHeight = pSrcDib->GetDibInfo()->m_bmHeight;
	m_DibInfo.m_bmWidth = pSrcDib->GetDibInfo()->m_bmWidth;
	m_DibInfoBack.m_bmWidth = m_DibInfo.m_bmWidth;
	m_DibInfoBack.m_bmHeight = m_DibInfo.m_bmHeight;
	m_DibInfo.m_nType = pSrcDib->GetDibInfo()->m_nType;

	return S_OK;
}

HRESULT CFastDIB::LoadFromDIBEX(CFastDIB *pSrcDib)
{
	if (pSrcDib == NULL)
		return E_FAIL;

	_CHECKTYPE(pSrcDib->GetDibInfo(), FDIBTYPE_RGBA, TCM_BELONG);
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_DOUBLEBUF, TCM_BELONG);

	m_DibInfo.m_bmHeight = pSrcDib->GetDibInfo()->m_bmHeight;
	m_DibInfo.m_bmWidth = pSrcDib->GetDibInfo()->m_bmWidth;
	m_DibInfoBack.m_bmWidth = m_DibInfo.m_bmWidth;
	m_DibInfoBack.m_bmHeight = m_DibInfo.m_bmHeight;

	memcpy(m_DibInfo.m_pvData, pSrcDib->GetDibInfo()->m_pvData, CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight*4);
	memcpy(m_DibInfoBack.m_pvData, pSrcDib->GetDibInfo()->m_pvData, CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight*4);

	return S_OK;
}

void CFastDIB::FreeDIB()
{
	if (m_DibInfo.m_pvData != NULL)
		delete []m_DibInfo.m_pvData;
	if (m_DibInfoBack.m_pvData != NULL)
		delete []m_DibInfoBack.m_pvData;
	ZeroMemory(&m_DibInfo, sizeof FASTDIB);
	ZeroMemory(&m_DibInfoBack, sizeof FASTDIB);
}

HRESULT CFastDIB::LoadFromHBitmap(HBITMAP hSrcBitmap)
{
	if ((m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)&&(!CFastDIB::m_bInited))
		return E_NOTINIT;
	if (hSrcBitmap == NULL)
		return E_FAIL;

	BITMAP tmpBitmap;
	if (GetObject(hSrcBitmap, sizeof BITMAP, &tmpBitmap)!=0)
	{
		if ((tmpBitmap.bmBitsPixel!=32)&&(tmpBitmap.bmBitsPixel!=8))
			return E_UNSUPORTCOLOR;
		if ((tmpBitmap.bmBitsPixel == 8)&&(m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF))
			return E_UNSUPORTCOLOR;

		if (m_DibInfo.m_nType!=FDIBTYPE_DOUBLEBUF)
		{
			int bitspixel = 0;
			int bitspixelOld = 0;
			if (m_DibInfo.m_nType == FDIBTYPE_RGBA)
				bitspixelOld = 32;
			else
				bitspixelOld = 8;

			if (tmpBitmap.bmBitsPixel==32)
			{
				bitspixel = 32;
				if (m_DibInfo.m_bmWidth*m_DibInfo.m_bmHeight*bitspixelOld < tmpBitmap.bmWidth*tmpBitmap.bmHeight*bitspixel)
				{
					delete []m_DibInfo.m_pvData;
					m_DibInfo.m_pvData = new DWORD[tmpBitmap.bmWidth*tmpBitmap.bmHeight];
				}
				m_DibInfo.m_nType = FDIBTYPE_RGBA;
				memcpy(m_DibInfo.m_pvData, tmpBitmap.bmBits, tmpBitmap.bmWidthBytes*tmpBitmap.bmHeight);
			}
			if (tmpBitmap.bmBitsPixel==8)
			{
				bitspixel = 8;
				if (m_DibInfo.m_bmWidth*m_DibInfo.m_bmHeight*bitspixelOld < tmpBitmap.bmWidth*tmpBitmap.bmHeight*bitspixel)
				{
					delete []m_DibInfo.m_pvData;
					m_DibInfo.m_pvData = new BYTE[tmpBitmap.bmWidth*tmpBitmap.bmHeight];
				}
				BYTE *pDst = (BYTE*)m_DibInfo.m_pvData;
				BYTE *pSrc = (BYTE*)tmpBitmap.bmBits;
				m_DibInfo.m_nType = FDIBTYPE_8BITBUF;

				int BytesPerScanline = 0;
				if (tmpBitmap.bmWidthBytes%4 == 0)
					BytesPerScanline = tmpBitmap.bmWidthBytes;
				else
					BytesPerScanline = tmpBitmap.bmWidthBytes+2;

				for (int j=0; j<tmpBitmap.bmHeight; j++)
				{
					memcpy(pDst, pSrc, tmpBitmap.bmWidth);
					pDst +=tmpBitmap.bmWidth;
					pSrc +=BytesPerScanline;
				} 
			}
		}
		else
		{
			memcpy(m_DibInfo.m_pvData, tmpBitmap.bmBits, tmpBitmap.bmWidthBytes*tmpBitmap.bmHeight);
			memcpy(m_DibInfoBack.m_pvData, tmpBitmap.bmBits, tmpBitmap.bmWidthBytes*tmpBitmap.bmHeight);
		}

		m_DibInfo.m_bmHeight = tmpBitmap.bmHeight;
		m_DibInfo.m_bmWidth = tmpBitmap.bmWidth;
		m_DibInfoBack.m_bmWidth = m_DibInfo.m_bmWidth;
		m_DibInfoBack.m_bmHeight = m_DibInfo.m_bmHeight;

		return S_OK;
	}
	else
		return E_FAIL;
}

HRESULT CFastDIB::LoadFromBuf(FASTDIB *lpDibInfo, FASTDIB *lpDibInfoBack)
{
	memcpy(GetDibInfo(), lpDibInfo, sizeof FASTDIB);
	memcpy(GetBackDibInfo(), lpDibInfoBack, sizeof FASTDIB);
	return S_OK;
}

HRESULT CFastDIB::FastBlt(HDC hdc, int nXDst, int nYDst, int Width, int Height, int nXSrc, int nYSrc)
{
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	if (hdc == NULL)
		return E_FAIL;
	if (Width == 0) Width = m_DibInfo.m_bmWidth;
	if (Height == 0) Height = m_DibInfo.m_bmHeight;

	HDC htmpdc = CreateCompatibleDC(hdc);

	BITMAPINFO bmi;
	ZeroMemory(&bmi, sizeof BITMAPINFO);
	bmi.bmiHeader.biSize = sizeof BITMAPINFOHEADER;
	bmi.bmiHeader.biWidth = Width;
	bmi.bmiHeader.biHeight = Height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void *pvBits = NULL;
	HBITMAP htmpBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0); 
	// ::SetBitmapBits(htmpBitmap, Width*Height*4, m_DibInfo.m_pvData);
	BITMAP tmpBitmap;
	GetObject(htmpBitmap, sizeof BITMAP, &tmpBitmap);
	memcpy(tmpBitmap.bmBits, m_DibInfo.m_pvData, (Width*Height)<<2);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(htmpdc, htmpBitmap);
	BitBlt(hdc, nXDst, nYDst, Width, Height, htmpdc, nXSrc, nYSrc, SRCCOPY);
	SelectObject(htmpdc, hOldBitmap);
	DeleteObject(htmpBitmap);
	DeleteDC(htmpdc);

	return S_OK;
}

HRESULT CFastDIB::Draw(CFastDIB *pDstDIB, UINT uDrawMode, int nXDst, int nYDst, int nWidth, int nHeight, int nXSrc, int nYSrc, 
					   BYTE nAlphaValue, BYTE nBoardWidth, DWORD nBoardColor)
{
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	_CHECKTYPE(pDstDIB->GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	if (nWidth == 0) nWidth = m_DibInfo.m_bmWidth-nXSrc;
	if (nHeight== 0) nHeight = m_DibInfo.m_bmHeight-nYSrc;

	int i=0, j=0; 

	int SrcHeight = m_DibInfo.m_bmHeight;
	int SrcWidth = m_DibInfo.m_bmWidth;
	int DstHeight = pDstDIB->GetBackDibInfo()->m_bmHeight;
	int DstWidth = pDstDIB->GetBackDibInfo()->m_bmWidth;

	DWORD *pSrc = NULL;
	DWORD *pDst = NULL;
	DWORD *pDst2 = NULL;

	////////////////////////////////////////////////////////

	if (nXDst<0)
	{
		nWidth += nXDst;
		nXSrc -= nXDst;
		nXDst = 0;
	}
	if (nYDst<0)
	{
		nHeight += nYDst;
		nYSrc -= nYDst;
		nYDst = 0;
	}
	int tmpx = (nWidth + nXDst - DstWidth);
	if (tmpx>0)
		nWidth -= tmpx;
	int tmpy = (nHeight + nYDst - DstHeight);
	if (tmpy>0)
		nHeight -= tmpy;
	////////裁剪DstZone///////////

	if (nXSrc<0)
	{
		nWidth += nXSrc;
		nXSrc = 0;
	}
	if (nYSrc<0)
	{
		nHeight += nYSrc;
		nYSrc = 0;
	}
	int tmpx2 = (nWidth + nXSrc - SrcWidth);

	if (tmpx2>0)
		nWidth -= tmpx2;

	int tmpy2 = (nHeight + nYSrc - SrcHeight);
	if (tmpy2>0)
		nHeight -= tmpy2;

	///////////////////区域裁剪////////////////////////////

	if ((uDrawMode & DRAWMODE_ALPHABLEND)!=DRAWMODE_ALPHABLEND)
		nAlphaValue = 255;

	if ((pDstDIB->GetDibInfo()->m_nType == FDIBTYPE_DOUBLEBUF)&&((uDrawMode&DRAWMODE_FROMBACK)==DRAWMODE_FROMBACK))
		pSrc = (DWORD *)m_DibInfoBack.m_pvData + (SrcHeight-nYSrc-1)*SrcWidth + nXSrc;
	//pSrc = (DWORD *)m_DibInfo.m_pvData + (SrcHeight-nYSrc-1)*SrcWidth + nXSrc;
	else
		pSrc = (DWORD *)m_DibInfo.m_pvData + (SrcHeight-nYSrc-1)*SrcWidth + nXSrc;

	pDst2 = (DWORD *)(pDstDIB->GetDibInfo()->m_pvData) + (DstHeight-nYDst-1)*DstWidth + nXDst;
	if (pDstDIB->GetDibInfo()->m_nType == FDIBTYPE_DOUBLEBUF)
		pDst = (DWORD *)(pDstDIB->GetBackDibInfo()->m_pvData) + (DstHeight-nYDst-1)*DstWidth + nXDst;
	else
		pDst = (DWORD *)(pDstDIB->GetDibInfo()->m_pvData) + (DstHeight-nYDst-1)*DstWidth + nXDst;

	UINT DrawMode = uDrawMode & 0x0000FFFF;

	DRAWPARAM dpm;
	ZeroMemory(&dpm, sizeof DRAWPARAM);
	dpm.nXDst = nXDst;
	dpm.nYDst = nYDst;
	dpm.nWidth = nWidth;
	dpm.nHeight = nHeight;
	dpm.nXSrc = nXSrc;
	dpm.nYSrc = nYSrc;
	dpm.nDstWidth = DstWidth;
	dpm.nDstHeight = DstHeight;
	dpm.nSrcWidth = SrcWidth;
	dpm.nSrcHeight = SrcHeight;
	dpm.pDst = pDst;
	dpm.pDst2 = pDst2;
	dpm.pSrc = pSrc;
	dpm.nAlphaValue = nAlphaValue;
	dpm.nBoardWidth = nBoardWidth;
	dpm.nBoardColor = nBoardColor;

	if (uDrawMode == DRAWMODE_COPY)
	{
		_DrawCopy(&dpm);
	}
	else
	{
		switch (DrawMode)
		{
		case DRAWMODE_IGNOREALPHA:
			_DrawIgnoreAlpha(&dpm);
			break;
		case DRAWMODE_NORMAL:
			_DrawNormal(&dpm);
			break;
		case DRAWMODE_ADDSATURATION:
			_DrawAddSaturation(&dpm);
			break;
		case DRAWMODE_SUBSATURATION:
			_DrawSubSaturation(&dpm);
			break;
		case DRAWMODE_XSUBSATURATION:
			_DrawSubSaturationFlipAlpha(&dpm);
			break;
		case DRAWMODE_XADDSATURATION:
			_DrawAddSaturationFlipAlpha(&dpm);
			break;
		case DRAWMODE_FLIPALPHA:
			_DrawFlipAlpha(&dpm);
		}
	}
	return S_OK;
}

void CFastDIB::_DrawCopy(DRAWPARAM *pdpm)
{
	DWORD *pCurrentSrcp = pdpm->pSrc;
	DWORD *pCurrentDstp = pdpm->pDst2;
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;

	DWORD nWidth4 = nWidth<<2;
	for (int j=0; j<(int)nHeight; j++)
	{
		memcpy(pCurrentDstp, pCurrentSrcp, nWidth4);
		pCurrentSrcp -= pdpm->nSrcWidth;
		pCurrentDstp -= pdpm->nDstWidth;
	}
}

void CFastDIB::_DrawIgnoreAlpha(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = DWORD(pdpm->pSrc);
	DWORD CurrentDst = DWORD(pdpm->pDst);
	DWORD CurrentDst2 = DWORD(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	DWORD nBoardColor = pdpm->nBoardColor;
	BYTE nAlphaValue = pdpm->nAlphaValue;
	BYTE nBoardWidth = pdpm->nBoardWidth;
	DWORD dwBoardWidth = pdpm->nBoardWidth; 

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;
	__int64 nTest64 = 0x00FF00FF00FF00FF;

	__asm{
		mov j, 0

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		movd mm0, [EBX]

		////////////////////////////////////////////////////////
		////////////////画色边////////////////////////////////// 
		mov EAX, 0
			mov AL, nBoardWidth
			cmp j, EAX
			jb LABEL1
			cmp i, EAX
			jb LABEL1
			mov EAX, nWidth
			sub EAX, dwBoardWidth
			cmp i, EAX
			jnb LABEL1
			mov EAX, nHeight
			sub EAX, dwBoardWidth
			cmp j, EAX
			jnb LABEL1

			movd [EDX], mm0
			jmp LABEL2

LABEL1: mov EAX, nBoardColor
		///////////////////////////////////////////////////////
		mov [EDX], EAX

LABEL2: add EBX, 4
		add ECX, 4
		add EDX, 4
		inc i
		mov EAX, i
		cmp EAX, nWidth
		jb LOOP2
		///////////////////////////////////////////////////////////////// 
		// pSrc = pSrc - 4*SrcWidth 
		mov EBX, CurrentSrc
		mov ECX, CurrentDst
		mov EDX, CurrentDst2

		sub EBX, SrcWidth4
		sub ECX, DstWidth4
		sub EDX, DstWidth4

		mov CurrentSrc, EBX
		mov CurrentDst, ECX
		mov CurrentDst2, EDX
		///////////////////////////////////////////////////////////////// 
		inc j
		mov EAX, j
		cmp EAX, nHeight
		jb LOOP1

		EMMS
	}
}

void CFastDIB::_DrawNormal(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = DWORD(pdpm->pSrc);
	DWORD CurrentDst = DWORD(pdpm->pDst);
	DWORD CurrentDst2 = DWORD(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	DWORD nBoardColor = pdpm->nBoardColor;
	BYTE nAlphaValue = pdpm->nAlphaValue;
	BYTE nBoardWidth = pdpm->nBoardWidth;
	DWORD dwBoardWidth = pdpm->nBoardWidth; 

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;

	__asm{
		mov j, 0

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2
			//mm2 = 源图片Alpha值
			movq mm3, AlphaMask
			psubusw mm3, mm2
			//mm3 = 256-源图片Alpha值
			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm0, mm2
			pmullw mm1, mm3

			paddusw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm7

			////////////////////////////////////////////////////////
			////////////////画色边////////////////////////////////// 
			mov EAX, 0
			mov AL, nBoardWidth
			cmp j, EAX
			jb LABEL1
			cmp i, EAX
			jb LABEL1
			mov EAX, nWidth
			sub EAX, dwBoardWidth
			cmp i, EAX
			jnb LABEL1
			mov EAX, nHeight
			sub EAX, dwBoardWidth
			cmp j, EAX
			jnb LABEL1

			movd [EDX], mm0
			jmp LABEL2

LABEL1: mov EAX, nBoardColor
		///////////////////////////////////////////////////////
		mov [EDX], EAX

LABEL2: add EBX, 4
		add ECX, 4
		add EDX, 4
		inc i
		mov EAX, i
		cmp EAX, nWidth
		jb LOOP2
		///////////////////////////////////////////////////////////////// 
		// pSrc = pSrc - 4*SrcWidth 
		mov EBX, CurrentSrc
		mov ECX, CurrentDst
		mov EDX, CurrentDst2

		sub EBX, SrcWidth4
		sub ECX, DstWidth4
		sub EDX, DstWidth4

		mov CurrentSrc, EBX
		mov CurrentDst, ECX
		mov CurrentDst2, EDX
		///////////////////////////////////////////////////////////////// 
		inc j
		mov EAX, j
		cmp EAX, nHeight
		jb LOOP1

		EMMS
	}
}

void CFastDIB::_DrawAddSaturation(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = DWORD(pdpm->pSrc);
	DWORD CurrentDst = DWORD(pdpm->pDst);
	DWORD CurrentDst2 = DWORD(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	BYTE nAlphaValue = pdpm->nAlphaValue;
	DWORD nBoardColor = pdpm->nBoardColor;
	BYTE nBoardWidth = pdpm->nBoardWidth;
	DWORD dwBoardWidth = pdpm->nBoardWidth;

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;

	__asm{
		mov j, 0
			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2

			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm0, mm2
			pmullw mm1, AlphaMask

			paddusw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm7

			////////////////画色边////////////////////////////////// 
			mov EAX, 0
			mov AL, nBoardWidth
			cmp j, EAX
			jb LABEL1
			cmp i, EAX
			jb LABEL1
			mov EAX, nWidth
			sub EAX, dwBoardWidth
			cmp i, EAX
			jnb LABEL1
			mov EAX, nHeight
			sub EAX, dwBoardWidth
			cmp j, EAX
			jnb LABEL1

			movd [EDX], mm0
			jmp LABEL2

LABEL1: mov EAX, nBoardColor
		///////////////////////////////////////////////////////
		movd [EDX], mm0

LABEL2: add EBX, 4
		add ECX, 4
		add EDX, 4
		inc i
		mov EAX, i
		cmp EAX, nWidth
		jb LOOP2

		mov EBX, CurrentSrc
		mov ECX, CurrentDst
		mov EDX, CurrentDst2
		sub EBX, SrcWidth4
		sub ECX, DstWidth4
		sub EDX, DstWidth4
		mov CurrentSrc, EBX
		mov CurrentDst, ECX
		mov CurrentDst2, EDX

		inc j
		mov EAX, j
		cmp EAX, nHeight
		jb LOOP1

		EMMS
	}
}

void CFastDIB::_DrawSubSaturation(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = (DWORD)(pdpm->pSrc);
	DWORD CurrentDst = (DWORD)(pdpm->pDst);
	DWORD CurrentDst2 = (DWORD)(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	BYTE nAlphaValue = pdpm->nAlphaValue;

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;


	__asm{
		mov j, 0
			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2

			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm0, mm2
			pmullw mm1, AlphaMask

			psubusw mm1, mm0
			psrlw mm1, 8

			packuswb mm1, mm7

			movd [EDX], mm1

			add EBX, 4
			add ECX, 4
			add EDX, 4
			inc i
			mov EAX, i
			cmp EAX, nWidth
			jb LOOP2

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
			sub EBX, SrcWidth4
			sub ECX, DstWidth4
			sub EDX, DstWidth4
			mov CurrentSrc, EBX
			mov CurrentDst, ECX
			mov CurrentDst2, EDX

			inc j
			mov EAX, j
			cmp EAX, nHeight
			jb LOOP1

			EMMS
	}
}

void CFastDIB::_DrawFlipAlpha(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = (DWORD)(pdpm->pSrc);
	DWORD CurrentDst = (DWORD)(pdpm->pDst);
	DWORD CurrentDst2 = (DWORD)(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	BYTE nAlphaValue = pdpm->nAlphaValue;

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;

	__asm{
		mov j, 0
			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2

			movq mm3, AlphaMask
			psubusw mm3, mm2

			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm0, mm3
			pmullw mm1, mm2

			paddusw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm7

			movd [EDX], mm0

			add EBX, 4
			add ECX, 4
			add EDX, 4
			inc i
			mov EAX, i
			cmp EAX, nWidth
			jb LOOP2

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
			sub EBX, SrcWidth4
			sub ECX, DstWidth4
			Sub EDX, DstWidth4
			mov CurrentSrc, EBX
			mov CurrentDst, ECX
			mov CurrentDst2, EDX

			inc j
			mov EAX, j
			cmp EAX, nHeight
			jb LOOP1

			EMMS
	}
}

void CFastDIB::_DrawAddSaturationFlipAlpha(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = (DWORD)(pdpm->pSrc);
	DWORD CurrentDst = (DWORD)(pdpm->pDst);
	DWORD CurrentDst2 = (DWORD)(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	BYTE nAlphaValue = pdpm->nAlphaValue;

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;

	__asm{
		mov j, 0
			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2

			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm1, mm2
			pmullw mm0, AlphaMask

			paddusw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm7

			movd [EDX], mm0

			add EBX, 4
			add ECX, 4
			add EDX, 4
			inc i
			mov EAX, i
			cmp EAX, nWidth
			jb LOOP2

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
			sub EBX, SrcWidth4
			sub ECX, DstWidth4
			sub EDX, DstWidth4
			mov CurrentSrc, EBX
			mov CurrentDst, ECX
			mov CurrentDst2, EDX

			inc j
			mov EAX, j
			cmp EAX, nHeight
			jb LOOP1

			EMMS
	}
}

void CFastDIB::_DrawSubSaturationFlipAlpha(DRAWPARAM *pdpm)
{
	DWORD CurrentSrc = (DWORD)(pdpm->pSrc);
	DWORD CurrentDst = (DWORD)(pdpm->pDst);
	DWORD CurrentDst2 = (DWORD)(pdpm->pDst2);
	DWORD nWidth = pdpm->nWidth;
	DWORD nHeight = pdpm->nHeight;
	DWORD DstWidth4 = pdpm->nDstWidth<<2;
	DWORD SrcWidth4 = pdpm->nSrcWidth<<2;
	BYTE nAlphaValue = pdpm->nAlphaValue;

	int i=0, j=0;

	__int64 AlphaMask = 0x0100010001000100;

	__asm{
		mov j, 0
			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
LOOP1: 
		mov i, 0

LOOP2: 
		pxor mm7, mm7
			mov EAX, [EBX]
		movd mm0, EAX
			movd mm1, [ECX]

		shr EAX, 24
			mul BYTE PTR nAlphaValue
			shr EAX, 8

			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2

			punpcklbw mm0, mm7
			punpcklbw mm1, mm7

			pmullw mm1, mm2
			pmullw mm0, AlphaMask

			psubusw mm0, mm1
			psrlw mm0, 8

			packuswb mm0, mm7

			movd [EDX], mm0

			add EBX, 4
			add ECX, 4
			add EDX, 4
			inc i
			mov EAX, i
			cmp EAX, nWidth
			jb LOOP2

			mov EBX, CurrentSrc
			mov ECX, CurrentDst
			mov EDX, CurrentDst2
			sub EBX, SrcWidth4
			sub ECX, DstWidth4
			sub EDX, DstWidth4
			mov CurrentSrc, EBX
			mov CurrentDst, ECX
			mov CurrentDst2, EDX

			inc j
			mov EAX, j
			cmp EAX, nHeight
			jb LOOP1

			EMMS
	}
}

HRESULT CFastDIB::CreateDIB(DWORD dwWidth, DWORD dwHeight, FASTDIBTYPE fdt)
{
	if (((fdt&FDIBTYPE_DOUBLEBUF) == FDIBTYPE_DOUBLEBUF)&&(!CFastDIB::m_bInited))
		return E_NOTINIT;
	if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
	{
		if (m_DibInfo.m_pvData!=NULL)
		{
			delete []m_DibInfo.m_pvData;
			m_DibInfo.m_pvData = NULL;
		}
	}
	if ((m_DibInfo.m_nType&FDIBTYPE_NULLBACK)!=FDIBTYPE_NULLBACK)
	{
		if (m_DibInfoBack.m_pvData != NULL)
		{
			delete []m_DibInfoBack.m_pvData;
			m_DibInfoBack.m_pvData = NULL;
		}
	}
	m_DibInfo.m_bmWidth = dwWidth;
	m_DibInfo.m_bmHeight = dwHeight;
	m_DibInfoBack.m_bmWidth = dwWidth;
	m_DibInfoBack.m_bmHeight = dwHeight;
	m_DibInfo.m_nType = fdt;
	FASTDIBTYPE tmpType = (m_DibInfo.m_nType&0x0000FFFF);
	switch (tmpType)
	{
	case FDIBTYPE_RGBA:
		if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
		{
			m_DibInfo.m_pvData = new DWORD[dwWidth * dwHeight];
		}
		break;
	case FDIBTYPE_8BITBUF:
		if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
		{
			m_DibInfo.m_pvData = new BYTE[dwWidth * dwHeight];
		}
		break;
	case FDIBTYPE_16BITBUF:
		if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
		{
			m_DibInfo.m_pvData = new WORD[dwWidth * dwHeight];
		}
		break;
	case FDIBTYPE_DOUBLEBUF:
		if ((m_DibInfo.m_nType&FDIBTYPE_NULLFRONT)!=FDIBTYPE_NULLFRONT)
		{
			m_DibInfo.m_pvData = new DWORD[CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight];
		}
		if ((m_DibInfo.m_nType&FDIBTYPE_NULLBACK)!=FDIBTYPE_NULLBACK)
		{
			m_DibInfoBack.m_pvData = new DWORD[CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight];
		}
	}
	return S_OK;
}

HRESULT CFastDIB::WaterFX(CFastDIB *pCurrentBuf, CFastDIB *pNextBuf)
{
	_CHECKTYPE(pCurrentBuf->GetDibInfo(), FDIBTYPE_8BITBUF, TCM_BELONG);
	_CHECKTYPE(pNextBuf->GetDibInfo(), FDIBTYPE_8BITBUF, TCM_BELONG);

	if (pCurrentBuf->GetDibInfo()->m_bmHeight != pNextBuf->GetDibInfo()->m_bmHeight)
		return E_FAIL;
	if (pCurrentBuf->GetDibInfo()->m_bmWidth != pNextBuf->GetDibInfo()->m_bmWidth)
		return E_FAIL;

	DWORD dwWidth = pNextBuf->GetDibInfo()->m_bmWidth;
	DWORD dwHeight= pNextBuf->GetDibInfo()->m_bmHeight;


	BYTE *pCurrent[5] = {NULL, NULL, NULL, NULL, NULL};

	BYTE *pNext = (BYTE *)(pNextBuf->GetDibInfo()->m_pvData);
	pCurrent[0] = (BYTE *)(pCurrentBuf->GetDibInfo()->m_pvData);

	for (int j=0; j<(int)dwHeight; j++)
	{
		for (int i=0; i<(int)dwWidth; i++)
		{
			if (j>1) 
				pCurrent[1] = pCurrent[0]-dwWidth;
			else
				pCurrent[1] = pCurrent[0];

			if (i<(int)dwWidth-1) 
				pCurrent[2] = pCurrent[0]+1;
			else
				pCurrent[2] = pCurrent[0];

			if (j<(int)dwHeight-1) 
				pCurrent[3] = pCurrent[0]+dwWidth;
			else
				pCurrent[3] = pCurrent[0];

			if (i>1) 
				pCurrent[4] = pCurrent[0]-1;
			else
				pCurrent[4] = pCurrent[0];


			short tmpint = ((*pCurrent[1] + *pCurrent[2] + *pCurrent[3] + *pCurrent[4])>>1) - (*pNext);
			if (tmpint < 0)
				*pNext = 0;
			else
				*pNext = (BYTE)tmpint;

			BYTE tmpn = ((*pNext)>>5);
			*pNext -= tmpn;

			pCurrent[0]++;
			pNext++;
		}
	}
	BYTE *p = (BYTE *)pCurrentBuf->GetDibInfo()->m_pvData;
	pCurrentBuf->GetDibInfo()->m_pvData = pNextBuf->GetDibInfo()->m_pvData;
	pNextBuf->GetDibInfo()->m_pvData = p;
	//memcpy(pCurrentBuf->GetDibInfo()->m_pvData, pNextBuf->GetDibInfo()->m_pvData, (pNextBuf->GetDibInfo()->m_bmWidth)*(pNextBuf->GetDibInfo()->m_bmHeight));
	return S_OK;
}

HRESULT CFastDIB::WaterFX16(CFastDIB *pCurrentBuf, CFastDIB *pNextBuf)
{
	_CHECKTYPE(pCurrentBuf->GetDibInfo(), FDIBTYPE_16BITBUF, TCM_BELONG);
	_CHECKTYPE(pNextBuf->GetDibInfo(), FDIBTYPE_16BITBUF, TCM_BELONG);

	if (pCurrentBuf->GetDibInfo()->m_bmHeight != pNextBuf->GetDibInfo()->m_bmHeight)
		return E_FAIL;
	if (pCurrentBuf->GetDibInfo()->m_bmWidth != pNextBuf->GetDibInfo()->m_bmWidth)
		return E_FAIL;

	DWORD dwWidth = pNextBuf->GetDibInfo()->m_bmWidth;
	DWORD dwHeight= pNextBuf->GetDibInfo()->m_bmHeight;

	WORD *pCurrent[5] = {NULL, NULL, NULL, NULL, NULL};

	WORD *pNext = (WORD *)(pNextBuf->GetDibInfo()->m_pvData);
	pCurrent[0] = (WORD *)(pCurrentBuf->GetDibInfo()->m_pvData);

	for (int j=0; j<(int)dwHeight; j++)
	{
		for (int i=0; i<(int)dwWidth; i++)
		{
			if (j>1) 
				pCurrent[1] = pCurrent[0]-dwWidth;
			else
				pCurrent[1] = pCurrent[0];

			if (i<(int)dwWidth-1) 
				pCurrent[2] = pCurrent[0]+1;
			else
				pCurrent[2] = pCurrent[0];

			if (j<(int)dwHeight-1) 
				pCurrent[3] = pCurrent[0]+dwWidth;
			else
				pCurrent[3] = pCurrent[0];

			if (i>1) 
				pCurrent[4] = pCurrent[0]-1;
			else
				pCurrent[4] = pCurrent[0];

			short tmpint = ((*pCurrent[1] + *pCurrent[2] + *pCurrent[3] + *pCurrent[4])>>1) - (*pNext);
			if (tmpint < 0)
				*pNext = 0;
			else
				*pNext = (BYTE)tmpint;

			BYTE tmpn = ((*pNext)>>5);
			*pNext -= tmpn;

			pCurrent[0]++;
			pNext++;
		}
	}
	WORD *p = (WORD *)pCurrentBuf->GetDibInfo()->m_pvData;
	pCurrentBuf->GetDibInfo()->m_pvData = pNextBuf->GetDibInfo()->m_pvData;
	pNextBuf->GetDibInfo()->m_pvData = p;

	return S_OK;
}

HRESULT CFastDIB::DistortByDIB(int x, int y, CFastDIB *pDIB)
{
	_CHECKTYPE(pDIB->GetDibInfo(), FDIBTYPE_8BITBUF, TCM_BELONG);
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	//////////////入口检查////////////////////////
	int DstWidth = m_DibInfoBack.m_bmWidth;
	int DstHeight = m_DibInfoBack.m_bmHeight; 
	int SrcWidth = pDIB->GetDibInfo()->m_bmWidth;
	int SrcHeight = pDIB->GetDibInfo()->m_bmHeight;
	int nWidth = SrcWidth;
	int nHeight = SrcHeight;
	if (x<0) 
	{
		nWidth += x;
		x = 0;
	}
	if (y<0)
	{
		nHeight += y;
		y = 0;
	}
	if (nWidth + x>DstWidth)
		nWidth = DstWidth-x;
	if (nHeight+ y>DstHeight)
		nHeight= DstHeight-y;
	/////////////区域裁减///////////////////////////
	DWORD *pSrc = NULL;
	DWORD *tmpBuf = NULL;
	if (m_DibInfo.m_nType!=FDIBTYPE_DOUBLEBUF)
	{
		tmpBuf = new DWORD[DstWidth*DstHeight];
		memcpy((void *)tmpBuf, (void *)m_DibInfo.m_pvData, DstWidth*DstHeight*4);
		pSrc = tmpBuf;
	}
	else
	{
		tmpBuf = (DWORD*)m_DibInfoBack.m_pvData;
		pSrc = tmpBuf;
	}
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData; 
	BYTE *pMap[5] = {(BYTE *)pDIB->GetDibInfo()->m_pvData, NULL, NULL, NULL, NULL};
	/////////////内存分配/////////////////////////////
	for (int j=2; j<nHeight-1; j++)
	{
		pMap[0] = (BYTE *)pDIB->GetDibInfo()->m_pvData+(SrcHeight-j)*SrcWidth;
		pSrc = (DWORD *)tmpBuf + (DstHeight-j-y)*DstWidth+x;
		pDst = (DWORD *)m_DibInfo.m_pvData + (DstHeight-j-y)*DstWidth+x;
		for (int i=0; i<nWidth; i++)
		{
			if (j>0) 
				pMap[1] = pMap[0]-SrcWidth;
			else
				pMap[1] = pMap[0];

			if (i<(int)SrcWidth) 
				pMap[2] = pMap[0]+1;
			else
				pMap[2] = pMap[0];

			if (j<(int)SrcHeight) 
				pMap[3] = pMap[0]+SrcWidth;
			else
				pMap[3] = pMap[0];

			if (i>0) 
				pMap[4] = pMap[0]-1;
			else
				pMap[4] = pMap[0];

			int dify = (*pMap[3])-(*pMap[1]);
			int difx = (*pMap[2])-(*pMap[4]);

			if ((dify>0)&&(dify>j+y)) dify = j+y;
			if ((dify<0)&&(dify>=(DstHeight-j-y))) dify = DstHeight-j-y;
			if ((difx<0)&&(difx>i+x)) difx = i+x;
			if ((difx>0)&&(difx>=(DstWidth-i-x))) difx = DstWidth-i-x;

			*pDst = *((DWORD *)pSrc + dify * DstWidth + difx);

			pMap[0]++;
			pDst ++;
			pSrc ++; 
		}
	}
	//////////////计算/////////////////////
	if (m_DibInfo.m_nType!=FDIBTYPE_DOUBLEBUF)
		delete []tmpBuf;
	return S_OK;
}

HRESULT CFastDIB::DistortByDIB16(int x, int y, CFastDIB *pDIB)
{
	_CHECKTYPE(pDIB->GetDibInfo(), FDIBTYPE_16BITBUF, TCM_BELONG);
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	int DstWidth = m_DibInfo.m_bmWidth;
	int DstHeight = m_DibInfo.m_bmHeight;

	int SrcWidth = pDIB->GetDibInfo()->m_bmWidth;
	int SrcHeight = pDIB->GetDibInfo()->m_bmHeight;

	int nWidth = SrcWidth;
	int nHeight = SrcHeight;

	if (x<0)
	{
		nWidth += x;
		x = 0;
	}
	if (y<0)
	{
		nHeight += y;
		y = 0;
	}
	int tmpx = (nWidth + x - DstWidth);
	if (tmpx>0)
		nWidth -= tmpx;
	int tmpy = (nHeight + y - DstHeight);
	if (tmpy>0)
		nHeight -= tmpy;


	DWORD *tmpBuf = NULL;
	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
		tmpBuf = (DWORD *)m_DibInfoBack.m_pvData;
	else
	{
		tmpBuf = new DWORD[DstWidth*DstHeight];
		memcpy((void *)tmpBuf, (void *)m_DibInfo.m_pvData, DstWidth*DstHeight*4);
	}


	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData;
	DWORD *p = pDst;
	DWORD *pSrc = tmpBuf;

	WORD *pMap[5] = {(WORD *)pDIB->GetDibInfo()->m_pvData, NULL, NULL, NULL, NULL};
	for (int j=0; j<nHeight; j++)
	{
		for (int i=0; i<nWidth; i++)
		{
			if (j>0) 
				pMap[1] = pMap[0]-SrcWidth;
			else
				pMap[1] = pMap[0];

			if (i<(int)SrcWidth-1) 
				pMap[2] = pMap[0]+1;
			else
				pMap[2] = pMap[0];

			if (j<(int)SrcHeight-1) 
				pMap[3] = pMap[0]+SrcWidth;
			else
				pMap[3] = pMap[0];

			if (i>0) 
				pMap[4] = pMap[0]-1;
			else
				pMap[4] = pMap[0];

			int dify = (*pMap[3])-(*pMap[1]);
			int difx = (*pMap[2])-(*pMap[4]);

			int tmpy2 = j+dify;
			int tmpx2 = i+difx;

			if (tmpy2<0)
				dify = 0;

			if (tmpy2>nHeight-1)
				dify = 0 ;

			if (tmpx2<0)
				difx = i;

			if (tmpx2>nWidth-1)
				difx = nWidth-i-1;

			*pDst = *(pSrc + dify * SrcWidth + difx);

			//*pDst = *pSrc;

			pMap[0]++;
			pDst ++;
			pSrc ++;

		}
		p += SrcWidth;
		pDst = p;
	}
	if (!(m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF))
		delete []tmpBuf;
	return S_OK;
}

HRESULT CFastDIB::ActivateBuf(int x, int y, BYTE nEnerge)
{
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_8BITBUF, TCM_BELONG);
	if (nEnerge == 0)
		return S_OK;

	if (x<0)
		x=0;
	if (y<0)
		y=0;

	BYTE *p = (BYTE *)m_DibInfo.m_pvData;
	p = p+(m_DibInfo.m_bmHeight-y-1)*(m_DibInfo.m_bmWidth)+x;

	int tmp = *p + nEnerge;
	if (tmp < 0)
		*p = 0;
	else
		if (tmp > 0xFF)
			*p = 0xFF;
		else
			*p = tmp;

	return S_OK;
}

HRESULT CFastDIB::ActivateBuf16(int x, int y, int r, WORD nEnerge)
{
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_16BITBUF, TCM_BELONG);
	if (nEnerge == 0)
		return S_OK;

	if (x<0)
		x=0;
	if (y<0)
		y=0;
	if (r<0)
		r=0;

	WORD *p = (WORD *)m_DibInfo.m_pvData;
	WORD *p1 = p;
	p = p+(m_DibInfo.m_bmHeight-y-1)*(m_DibInfo.m_bmWidth)+x;
	for (int j = 0; j<r; j++)
	{
		for (int i=0; i<r; i++)
		{
			int tmp = *p + nEnerge;
			if (tmp < 0)
				*p = 0;
			else
				*p = tmp;
			p++;
		}
		p1 += m_DibInfo.m_bmWidth;
		p = p1;
	}
	return S_OK;
}

HRESULT CFastDIB::CreateCompatibleDIB(CFastDIB *pDIB)
{
	if (pDIB == NULL)
		return E_EMPTYOBJECT;

	CreateDIB(pDIB->GetBackDibInfo()->m_bmWidth, pDIB->GetBackDibInfo()->m_bmHeight, pDIB->GetDibInfo()->m_nType);

	return S_OK;
}

DWORD CFastDIB::m_dwMaxWidth = 0;
DWORD CFastDIB::m_dwMaxHeight = 0;
BOOL CFastDIB::m_bInited = FALSE;
COLORTABLE CFastDIB::m_ct[32][32];
ANGLETABLE CFastDIB::m_at[3600];
DWORD CFastDIB::m_hsltable[262144];
int *CFastDIB::m_mapx = NULL;
int *CFastDIB::m_offsetx = NULL;
void CFastDIB::Initialize(DWORD dwMaxWidth, DWORD dwMaxHeight)
{
	int i,j;
	CFastDIB::m_dwMaxWidth = dwMaxWidth;
	CFastDIB::m_dwMaxHeight = dwMaxHeight;

	CFastDIB::m_mapx = new int [dwMaxWidth];
	CFastDIB::m_offsetx = new int [dwMaxWidth];

	for (i=0; i<32; i++)
	{
		for (j=0; j<32; j++)
		{
			CFastDIB::m_ct[i][j].m_part0 = (DWORD)(((31-i)*(31-j)/(float)(31*31))*128);
			CFastDIB::m_ct[i][j].m_part1 = (DWORD)((i*(31-j)/(float)(31*31))*128);
			CFastDIB::m_ct[i][j].m_part2 = (DWORD)((i*j/(float)(31*31))*128);
			CFastDIB::m_ct[i][j].m_part3 = 128 - CFastDIB::m_ct[i][j].m_part0 - CFastDIB::m_ct[i][j].m_part1 - CFastDIB::m_ct[i][j].m_part2;
		}
	}

	double tmpf = 3.1415926533/180;
	for (i=0; i<3600; i++)
	{
		CFastDIB::m_at[i].m_sinpart = (int)(sin((i/10.0)*tmpf)*1024);
		CFastDIB::m_at[i].m_cospart = (int)(cos((i/10.0)*tmpf)*1024);
	}

	for (i=0; i<262144; i++)
	{
		DWORD ncolor = i;
		BYTE B=(BYTE)(ncolor&0x3F);
		BYTE G=(BYTE)((ncolor>>6)&0x3F);
		BYTE R=(BYTE)((ncolor>>12)&0x3F);
		BYTE L=0;
		BYTE S=0;
		short H=0;

		BYTE nMax = R;
		if (nMax<G)
			nMax=G;
		if (nMax<B)
			nMax=B;

		BYTE nMin = R;
		if (nMin>G)
			nMin=G;
		if (nMin>B)
			nMin=B;
		if (nMax==nMin)
		{
			H=0;
			S=0;
			L=0;
		}
		else
		{
			L=(BYTE)(((nMax+nMin)/126.0)*240);
			S=0;
			H=0;
			if (L<120)
				S=(BYTE)(((nMax-nMin)/(float)(nMax+nMin))*240);
			else
				S=(BYTE)(((nMax-nMin)/(float)(126-nMax-nMin))*240);

			if (R==nMax)
				H=(short)((G-B)*60/(float)(nMax-nMin));
			else if (G==nMax)
				H=(short)((2+(B-R)/(float)(nMax-nMin))*60);
			else if (B==nMax)
				H=(short)((4+(R-G)/(float)(nMax-nMin))*60);
			if (H<0)
				H+=360;
			CFastDIB::m_hsltable[i] = (L<<24)|(S<<16)|(H);
		}
	}

	m_bInited = TRUE;
}

HRESULT CFastDIB::Stretch(double fxtime, double fytime, DWORD x, DWORD y, DWORD nWidth, DWORD nHeight, BOOL bBlur)
{
	/////////////////////////////
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	DWORD tmpdw = (DWORD)((m_DibInfoBack.m_bmWidth-x)*fxtime);
	if (nWidth>tmpdw)
		nWidth = tmpdw;

	tmpdw = (DWORD)((m_DibInfoBack.m_bmHeight-y)*fytime); 
	if (nHeight>tmpdw)
		nHeight = tmpdw;

	////////////裁剪/////////////
	DWORD *pSrc0 = NULL, *pSrc1 = NULL, *pSrc2 = NULL, *pSrc3 = NULL;
	DWORD *tmpBuf = NULL;
	DWORD SrcWidth = m_DibInfoBack.m_bmWidth, 
		SrcHeight = m_DibInfoBack.m_bmHeight;
	DWORD *pSource = NULL;

	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
	{
		tmpBuf = (DWORD *)m_DibInfo.m_pvData;
		pSource = (DWORD *)m_DibInfoBack.m_pvData;
	}
	else
	{
		tmpBuf = new DWORD[nWidth*nHeight];
		pSource = (DWORD *)m_DibInfo.m_pvData;
	}
	DWORD *pDst = tmpBuf;

	register int mapy = 0; 
	register int offsety = 0;
	int ytime = (int)(1/fytime*1024);
	int xtime = (int)(1/fxtime*1024);

	int tmpint;
	//////////局部变量///////////
	for (int j=nHeight-1; j>=0; j--)
	{
		tmpint = j*ytime;
		mapy = SrcHeight - (tmpint >>10) - y-1;
		if (bBlur)
		{
			offsety = tmpint & 0x000003FF;
			offsety >>= 5;
		}
		for (int i=0; i<(int)nWidth; i++)
		{
			if (j==(int)nHeight-1)
			{
				tmpint = i*xtime;
				m_mapx[i] = (tmpint >>10) + x;
				tmpint &= 0x000003FF;
				tmpint >>= 5;
				m_offsetx[i] = tmpint;
			}

			pSrc0 = pSource + (mapy)*SrcWidth + m_mapx[i];

			if (!bBlur)
			{
				*pDst = *pSrc0;
			}
			else
			{
				if (m_mapx[i]<(int)SrcWidth)
					pSrc1 = pSrc0 + 1;
				else
					pSrc1 = pSrc0;
				if ((mapy>1)&&(m_mapx[i]<(int)SrcWidth))
					pSrc2 = pSrc0 - SrcWidth + 1;
				else
					pSrc2 = pSrc0; 
				if (mapy>1)
					pSrc3 = pSrc0 - SrcWidth;
				else
					pSrc3 = pSrc0;

				COLORTABLE *tmpct = &m_ct[m_offsetx[i]][offsety];

				__asm{
					pxor mm7, mm7

						mov EBX, tmpct;
					mov EAX, pSrc0
						movd mm0, [EAX]
					punpcklbw mm0, mm7

						mov EAX, pSrc1
						movd mm1, [EAX]
					punpcklbw mm1, mm7

						movd mm2, [EBX]tmpct.m_part0
						movd mm3, [EBX]tmpct.m_part1
						punpcklwd mm2, mm2
						punpckldq mm2, mm2
						punpcklwd mm3, mm3
						punpckldq mm3, mm3

						pmullw mm0, mm2
						pmullw mm1, mm3

						paddusw mm0, mm1
						////////////////////////
						////////////////////////
						mov EAX, pSrc2
						movd mm4, [EAX]
					punpcklbw mm4, mm7

						mov EAX, pSrc3
						movd mm1, [EAX]
					punpcklbw mm1, mm7

						movd mm2, [EBX]tmpct.m_part2
						movd mm3, [EBX]tmpct.m_part3
						punpcklwd mm2, mm2
						punpckldq mm2, mm2
						punpcklwd mm3, mm3
						punpckldq mm3, mm3

						pmullw mm4, mm2
						pmullw mm1, mm3

						paddusw mm4, mm1
						///////////////////////
						///////////////////////
						paddusw mm0, mm4

						psrlw mm0, 7
						packuswb mm0, mm7

						mov EAX, pDst
						movd [EAX], mm0

						EMMS
				}
			}
			pDst++;
		}
	}

	if (!(m_DibInfo.m_nType==FDIBTYPE_DOUBLEBUF))
	{
		delete []m_DibInfo.m_pvData;
		m_DibInfo.m_pvData = tmpBuf;
		m_DibInfoBack.m_bmWidth = nWidth;
		m_DibInfoBack.m_bmHeight = nHeight;
	}
	m_DibInfo.m_bmWidth = nWidth;
	m_DibInfo.m_bmHeight = nHeight;

	return S_OK;
}

HRESULT CFastDIB::Rotation(int nangle, int cx, int cy, DWORD nWidth, DWORD nHeight, BOOL bBlur)
{
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	int mapx = 0; int mapy = 0;


	if (nWidth == 0) 
	{
		nWidth = m_DibInfoBack.m_bmWidth;
	}
	if (nHeight == 0) 
	{
		nHeight = m_DibInfoBack.m_bmHeight;
	}
	if (nWidth > nHeight)
		nHeight = nWidth;
	else
		nWidth = nHeight;
	if (nWidth > CFastDIB::m_dwMaxWidth)
		nWidth = CFastDIB::m_dwMaxWidth;
	if (nHeight > CFastDIB::m_dwMaxHeight)
		nHeight = CFastDIB::m_dwMaxHeight;

	int angle = 0;
	//////////裁剪////////////////
	angle = nangle%3600;
	if (angle < 0)
		angle = 3600-angle;
	/////////换算角度/////////////
	DWORD *pSource = NULL;
	DWORD *pDst = NULL;
	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
	{
		pSource = (DWORD*)m_DibInfoBack.m_pvData;
		pDst = (DWORD*)m_DibInfo.m_pvData;
	}
	else
	{
		pSource = (DWORD*)m_DibInfo.m_pvData;
		pDst = new DWORD[nWidth*nHeight];
	}

	DWORD *pSrc0 = NULL, *pSrc1 = NULL, *pSrc2 = NULL, *pSrc3 = NULL;
	int tmpint = 0;
	DWORD SrcWidth = m_DibInfoBack.m_bmWidth;
	DWORD SrcHeight = m_DibInfoBack.m_bmHeight;
	//////////////局部变量/////////////////////
	for (int j = 0; j<(int)nHeight; j++)
	{
		pDst = (DWORD*)m_DibInfo.m_pvData + (nHeight-j-1)*m_DibInfoBack.m_bmWidth;
		for (int i=0; i<(int)nWidth; i++)
		{
			tmpint = (i-cx)*m_at[angle].m_cospart-(j-cy)*m_at[angle].m_sinpart;
			mapx = (tmpint >> 10)+cx;
			register int offsetx = (tmpint & 0x3FF)>>5;
			tmpint = (j-cy)*m_at[angle].m_cospart+(i-cx)*m_at[angle].m_sinpart;
			mapy = (tmpint >> 10)+cy;
			register int offsety = (tmpint & 0x3FF)>>5;
			//计算坐标映射//
			if ((mapx<0)||(mapx>m_DibInfoBack.m_bmWidth-1)) 
			{
				*pDst = 0;
				pDst ++;
				continue;
			}
			if ((mapy<0)||(mapy>m_DibInfoBack.m_bmHeight-1))
			{
				*pDst = 0;
				pDst ++;
				continue;
			}
			//判断边界//
			pSrc0 = pSource + (nHeight - mapy -1)*m_DibInfoBack.m_bmWidth + mapx;
			if (!bBlur)
				*pDst = *pSrc0;
			//不插值//
			else
			{
				if (mapx<(int)nWidth)
					pSrc1 = pSrc0 + 1;
				else
					pSrc1 = pSrc0;
				if ((mapy<(int)nHeight-1)&&(mapx<(int)nWidth))
					pSrc2 = pSrc0 - SrcWidth + 1;
				else
					pSrc2 = pSrc0; 
				if (mapy<(int)nHeight-1)
					pSrc3 = pSrc0 - SrcWidth;
				else
					pSrc3 = pSrc0;
				//插值点//
				COLORTABLE *tmpct = &m_ct[offsetx][offsety];

				__asm{
					pxor mm7, mm7

						mov EBX, tmpct;
					mov EAX, pSrc0
						movd mm0, [EAX]
					punpcklbw mm0, mm7

						mov EAX, pSrc1
						movd mm1, [EAX]
					punpcklbw mm1, mm7

						movd mm2, [EBX]tmpct.m_part0
						movd mm3, [EBX]tmpct.m_part1
						punpcklwd mm2, mm2
						punpckldq mm2, mm2
						punpcklwd mm3, mm3
						punpckldq mm3, mm3

						pmullw mm0, mm2
						pmullw mm1, mm3

						paddusw mm0, mm1
						////////////////////////
						////////////////////////
						mov EAX, pSrc2
						movd mm4, [EAX]
					punpcklbw mm4, mm7

						mov EAX, pSrc3
						movd mm1, [EAX]
					punpcklbw mm1, mm7

						movd mm2, [EBX]tmpct.m_part2
						movd mm3, [EBX]tmpct.m_part3
						punpcklwd mm2, mm2
						punpckldq mm2, mm2
						punpcklwd mm3, mm3
						punpckldq mm3, mm3

						pmullw mm4, mm2
						pmullw mm1, mm3

						paddusw mm4, mm1
						///////////////////////
						///////////////////////
						paddusw mm0, mm4

						psrlw mm0, 7
						packuswb mm0, mm7

						mov EAX, pDst
						movd [EAX], mm0

						EMMS
				}
				//插值计算//
			}
			//插值//
			pDst++;
		}
	}
	m_DibInfo.m_bmWidth = nWidth;
	m_DibInfo.m_bmHeight = nHeight;

	if (!(m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF))
	{
		delete []m_DibInfo.m_pvData;
		m_DibInfo.m_pvData = pDst;
	}

	return S_OK;
}

HRESULT CFastDIB::Finalization()
{
	if (!m_bInited)
		return E_NOTINIT;
	delete []CFastDIB::m_mapx;
	delete []CFastDIB::m_offsetx;
	return S_OK;
}

HRESULT CFastDIB::SyncDoubleBuf()
{
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_DOUBLEBUF, TCM_BELONG);
	if (!m_bInited)
		return E_NOTINIT;

	memcpy(m_DibInfo.m_pvData, m_DibInfoBack.m_pvData, m_dwMaxWidth*m_dwMaxHeight*4);

	return S_OK;
}

HRESULT CFastDIB::BitmapWipe(CFastDIB *pBufDIB, BYTE nCurrentTime, BYTE nBoardWidth, BYTE nBoardWidthColor, 
							 BOOL bColorBoard, DWORD nBoardColor, DWORD x, DWORD y)
{
	_CHECKTYPE(pBufDIB->GetDibInfo(), FDIBTYPE_8BITBUF, TCM_BELONG);
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);

	//入口检查
	if (!bColorBoard)
	{
		nBoardWidthColor = 0;
		nBoardColor = 0;
	}
	else
	{
		if (nBoardWidth > 0)
		{
			if (nBoardWidthColor<nBoardWidth)
				nBoardWidth = nBoardWidthColor;
			else
				nBoardWidthColor = nBoardWidth;
		}
	}
	DWORD SrcWidth = pBufDIB->GetDibInfo()->m_bmWidth;
	DWORD SrcHeight= pBufDIB->GetDibInfo()->m_bmHeight;
	DWORD DstWidth = m_DibInfoBack.m_bmWidth;
	DWORD DstHeight= m_DibInfoBack.m_bmHeight;
	DWORD DstWidth4 = DstWidth << 2;
	DWORD DstHeight4= DstHeight<< 2;
	DWORD nWidth = SrcWidth;
	DWORD nHeight= SrcHeight;
	if (nWidth + x > DstWidth)
		nWidth = DstWidth-x;
	if (nHeight+ y > DstHeight)
		nHeight= DstHeight-y;
	BYTE *pMap = (BYTE *)pBufDIB->GetDibInfo()->m_pvData + (SrcHeight-1)*SrcWidth;
	DWORD *pSrc = NULL;
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData + (DstHeight-y-1)*DstWidth + x;
	DWORD *pSrcoffset = NULL;
	if (m_DibInfo.m_nType != FDIBTYPE_DOUBLEBUF)
	{
		pSrc = (DWORD *)m_DibInfo.m_pvData + (DstHeight-y-1)*DstWidth + x;
	}
	else
	{
		pSrc = (DWORD *)m_DibInfoBack.m_pvData + (DstHeight-y-1)*DstWidth + x;
	}
	BYTE bl[256];
	BYTE bl2[256];
	BYTE *pbl = bl, *pbl2 = bl2;
	BYTE nBoardWidthHalf = nBoardWidth>>1;
	BYTE nBoardWidthColorHalf = nBoardWidthColor>>1;

	for (int k = 0; k<nBoardWidth; k++)
	{
		bl[k] = (k*255/nBoardWidth);
	}

	for (int l = 0; l<nBoardWidthColor; l++)
	{

		if ((l<nBoardWidthColorHalf)||(nBoardWidth == 0))
			bl2[l] = 255;//nBoardWidth*255/nBoardWidthHalf;
		else
			bl2[l] = (nBoardWidthColor-l)*255/nBoardWidthColorHalf;
	}


	DWORD dwTmp = 0, dwTmp2 = 0;
	BYTE tmpByte = 0, tmpByte2=0, tmpByte3 = 0;
	DWORD dwCurrentTime = nCurrentTime;
	__int64 AlphaMask = 0x00FF00FF00FF00FF;
	int j=0, i=0;
	__asm{
		mov j, 0
			mov EBX, pMap
			mov ECX, pSrc
			mov EDX, pDst

LOOPj: 
		mov i, 0
LOOPi: 
		mov AL, BYTE PTR [EBX] 
		cmp AL, nCurrentTime
			jb LABEL1
			mov tmpByte, 0 
			jmp LABEL2
LABEL1: mov tmpByte, 255
		mov AH, nCurrentTime
		sub AH, AL 
		mov tmpByte2, AH
		cmp AH, nBoardWidth 
		jb LABEL3
		jmp LABEL4

LABEL3: mov EAX, 0
		mov AL, tmpByte2 
		add EAX, pbl
		mov AL, [EAX]
		mov tmpByte, AL

LABEL4: mov EAX, 0
		mov AL, tmpByte2
		add EAX, pbl2
		mov AL, [EAX]
		mov tmpByte3, AL

			mov AH, tmpByte2
			cmp AH, nBoardWidthColor
			jb LABEL5
			jmp LABEL2

LABEL5:
		pxor mm7, mm7
			movd mm0, nBoardColor
			movd mm1, [ECX]
		punpcklbw mm0, mm7
			punpcklbw mm1, mm7
			mov EAX, 0
			mov AL, tmpByte3
			movd mm2, EAX
			punpcklwd mm2, mm2
			punpckldq mm2, mm2
			movq mm3, AlphaMask
			psubusw mm3, mm2
			pmullw mm0, mm2
			pmullw mm1, mm3
			paddusw mm0, mm1
			psrlw mm0, 8
			packuswb mm0, mm0
			movd EAX, mm0
			EMMS

			// mov EAX, nBoardColor
			mov [EDX], EAX
			jmp LABEL6

LABEL2: mov EAX, [ECX]
		jmp LABEL7

LABEL6: mov EAX, [EDX]

LABEL7: mov dwTmp, EAX
			shr EAX, 24
			mul tmpByte
			shr AX, 8
			shl EAX, 24;
		and dwTmp, 0x00FFFFFF
			or EAX, dwTmp
			mov [EDX], EAX

			add EBX, 1
			add ECX, 4
			add EDX, 4

			inc i;
		mov EAX, i
			cmp EAX, nWidth
			jb LOOPi

			mov EBX, pMap
			mov ECX, pSrc
			mov EDX, pDst

			sub EBX, SrcWidth
			sub ECX, DstWidth4
			sub EDX, DstWidth4

			mov pMap, EBX
			mov pSrc, ECX
			mov pDst, EDX

			inc j
			mov EAX, j
			cmp EAX, nHeight
			jb LOOPj

	}
	return S_OK;
}

HRESULT CFastDIB::ColorTransparency(PCOLORTRANSPARENCY pctr)
{
	//检查入口
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	//.........
	//源内存、目标内存
	DWORD *pSrc = NULL;
	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
		pSrc = (DWORD *)m_DibInfoBack.m_pvData;
	else
		pSrc = (DWORD *)m_DibInfo.m_pvData;
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData; 
	//.........
	//局部变量
	DWORD *phsltable = CFastDIB::m_hsltable; //RGB转HSL色彩映射表
	int hslindex = 0; //RGB转HSL色彩映射表索引
	DWORD HSLValue = 0;
	WORD H = 0; 
	BYTE S = 0;
	BYTE L = 0;
	BYTE A = 0;
	//.........
	if (pctr->m_nBoardBlur>pctr->m_wHueRange)
		pctr->m_nBoardBlur = pctr->m_wHueRange;
	//计算色彩边缘柔化表、亮度高门柔化表、亮度低门柔化表
	BYTE bl[181];
	BYTE blHigh[255];
	BYTE blLow[255];
	int i=0;
	//色彩边缘柔化表
	if (pctr->m_bColorKey)
	{
		for (i=0; i<pctr->m_nBoardBlur; i++)
		{
			bl[i] = (BYTE)((i<<8)/pctr->m_nBoardBlur);
		}
	}
	//亮度高门柔化、低门柔化表
	if (pctr->m_bLightKey)
	{
		for (i = 0; i<pctr->m_nHighLevelBlur; i++)
			blHigh[i] = (BYTE)(((pctr->m_nHighLevelBlur-i)<<8)/pctr->m_nHighLevelBlur);
		for (i = 0; i<pctr->m_nLowLevelBlur; i++)
			blLow[i] = (BYTE)(((pctr->m_nLowLevelBlur-i)<<8)/pctr->m_nLowLevelBlur);
	}
	/////////////////////////////////////////////////////
	//抠象计算
	for (int j=0; j<m_DibInfoBack.m_bmHeight; j++)
	{
		for (i=0; i<m_DibInfoBack.m_bmWidth; i++)
		{
			//将8x4位色转换成6x3位色以作为RGB转HSL色彩映射表（近似）索引
			__asm{
				mov EAX, pSrc
					mov ECX, [EAX]; CH<<G CL<<B
					mov EBX, ECX
					shr EBX, 16; BL<<R
					mov A, BH

					mov EDX, 0
					mov EAX, 0
					shr BL, 2 
					movzx EDX, BL
					or EAX, EDX
					shl EAX, 6

					shr CH, 2
					movzx EDX, CH
					or EAX, EDX
					shl EAX, 6

					shr CL, 2
					movzx EDX, CL
					or EAX, EDX

					mov hslindex, EAX
			}
			//..........................
			//由索引通过查表获取当前颜色所对应的HSL值
			HSLValue = CFastDIB::m_hsltable[hslindex];
			H = (WORD)(HSLValue&0x0000FFFF);
			S = (BYTE)((HSLValue&0x00FF0000)>>16);
			L = (BYTE)((HSLValue&0xFF000000)>>24);
			//..........................
			BYTE nAlphaValue = 0xFF;
			BYTE nAlphaValueColor = 0xFF;
			BYTE nAlphaValueLight = 0xFF;
			BOOL bInColorKey = FALSE;
			//通过色键计算Alpha值
			if (pctr->m_bColorKey)
			{
				short nHueDif = abs(pctr->m_wHue-H);
				if (nHueDif>180) nHueDif = 360-nHueDif;
				if ((nHueDif<pctr->m_wHueRange)&&(S>pctr->m_nSaturation))
				{
					if (nHueDif>(pctr->m_wHueRange-pctr->m_nBoardBlur))
						nAlphaValueColor = bl[nHueDif-(pctr->m_wHueRange-pctr->m_nBoardBlur)];
					else
						nAlphaValueColor = 0;
					bInColorKey = TRUE; 
				}
			}
			//通过亮键计算Alpha值
			if (pctr->m_bLightKey)
			{
				BYTE nLightDifHigh = abs(L-pctr->m_nHighLevelDoor);
				BYTE nLightDifLow = abs(L-pctr->m_nLowLevelDoor);
				if ((L<pctr->m_nHighLevelDoor)&&(L>pctr->m_nLowLevelDoor))
				{
					if (nLightDifHigh<pctr->m_nHighLevelBlur)
					{
						nAlphaValueLight = blHigh[nLightDifHigh];
						nAlphaValueColor = 255;
					}
					else
						if (nLightDifLow<pctr->m_nLowLevelBlur)
						{
							nAlphaValueLight = blLow[nLightDifLow];
							nAlphaValueColor = 255;
						}
						else
							nAlphaValueLight = 0;
				}
				else
				{
					nAlphaValueColor = 255;
				}
			}
			//合并色键、亮键所产生的效果
			if ((pctr->m_bColorKey)&&(pctr->m_bLightKey))
			{
				if (bInColorKey)
					nAlphaValue = (nAlphaValueColor*nAlphaValueLight)>>8;
				else
					nAlphaValue = 255;
			}
			else
			{
				if (pctr->m_bColorKey)
					nAlphaValue = nAlphaValueColor;
				if (pctr->m_bLightKey)
					nAlphaValue = nAlphaValueLight;
			}
			//将色键、亮键的抠象结构应用到目标图象
			nAlphaValue = A*nAlphaValue>>8;
			*pDst = *pSrc;
			*pDst &= 0x00FFFFFF;
			*pDst |= (nAlphaValue<<24);

			pDst++;
			pSrc++;
		}
	}

	return S_OK;
}

HRESULT CFastDIB::Paint(PPAINTPARAM pppm)
{
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData;
	DWORD *pSrc = NULL;
	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
		pSrc = (DWORD *)m_DibInfoBack.m_pvData;
	else
		pSrc = (DWORD *)m_DibInfo.m_pvData;

	WORD H = 0;
	BYTE S = 0;
	BYTE L = 0;
	DWORD HSLValue;
	BYTE R = 0, G = 0, B = 0, A = 0;
	DWORD hslindex = 0;
	DWORD *ptmpcolor = new DWORD[CFastDIB::m_dwMaxWidth];
	DWORD dwTmpSrc = 0;
	for (int j=0; j<m_DibInfo.m_bmHeight; j++)
	{
		for (int i=0; i<m_DibInfo.m_bmWidth; i++)
		{
			//计算马赛克效果
			if ((pppm->m_nMosaicHorizontal>0)&&(pppm->m_nMosaicVertical>0))
			{
				if (j%pppm->m_nMosaicVertical==0)
				{
					if (i%pppm->m_nMosaicHorizontal==0)
						ptmpcolor[i] = *pSrc;
					else
						ptmpcolor[i] = ptmpcolor[i-1];
				}
				dwTmpSrc = ptmpcolor[i];
			}
			else
				dwTmpSrc = *pSrc;
			/////////////////////////////////////////////////////
			//RGB转换为HSL
			__asm{
				mov ECX, dwTmpSrc; CH<<G CL<<B
					mov EBX, ECX
					shr EBX, 16; BL<<R
					mov A, BH

					mov EDX, 0
					mov EAX, 0
					shr BL, 2 
					movzx EDX, BL
					or EAX, EDX
					shl EAX, 6

					shr CH, 2
					movzx EDX, CH
					or EAX, EDX
					shl EAX, 6

					shr CL, 2
					movzx EDX, CL
					or EAX, EDX

					mov hslindex, EAX
			}
			//..........................
			//由索引通过查表获取当前颜色所对应的HSL值
			HSLValue = CFastDIB::m_hsltable[hslindex];
			H = (WORD)(HSLValue&0x0000FFFF);
			S = (BYTE)((HSLValue&0x00FF0000)>>16);
			L = (BYTE)((HSLValue&0xFF000000)>>24);
			//..........................
			//色彩亮度处理/////////////////////////

			//Add Code here

			///////////////////////////////////////
			//HSL转回RGB
			double tmp1 = 0, tmp2 = 0, tmp3[3]={0, 0, 0};
			BYTE ncolor[3] = {0, 0, 0};
			double fS=S/240.0, fH=H/360.0, fL=L/240.0;
			if (S==0)
			{
				R = L;
				G = L;
				B = L;
			}
			else
			{
				if (L<0.5)
					tmp2 = fL*(1.0+fS);
				else
					tmp2 = fL+fS-fL*fS;

				tmp1 = 2.0*fL-tmp2;

				tmp3[0] = fH+0.33;
				if (tmp3[0]>1)
					tmp3[0] = tmp3[0]-1;
				tmp3[1] = fH;
				tmp3[2] = fH-0.33;
				if (tmp3[2]<0)
					tmp3[2] += 1;

				for (int k=0; k<3; k++)
				{
					if (tmp3[k]*6.0<1)
						ncolor[k] = (BYTE)((tmp1+(tmp2-tmp1)*6.0*tmp3[k])*255);
					else
						if (2.0*tmp3[k]<1)
							ncolor[k] = (BYTE)(tmp2*255);
						else
							if (3.0*tmp3[k]<2)
								ncolor[k] = (BYTE)((tmp1+(tmp2-tmp1)*0.67*tmp3[k]*6.0)*255);
				}

				dwTmpSrc = ((((0xFF00|ncolor[0])<<8)|ncolor[1])<<8)|ncolor[2];

			}
			///////////////////////////////////////
			*pDst = dwTmpSrc;

			pSrc++;
			pDst++;
		}
	}
	delete []ptmpcolor;
	return S_OK;
}
HRESULT CFastDIB::QuickPaint(PPAINTPARAM pppm)
{
	_CHECKTYPE(GetDibInfo(), (FDIBTYPE_RGBA|FDIBTYPE_DOUBLEBUF), TCM_BELONG);
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData;
	DWORD *pSrc = NULL;
	if (m_DibInfo.m_nType == FDIBTYPE_DOUBLEBUF)
		pSrc = (DWORD *)m_DibInfoBack.m_pvData;
	else
		pSrc = (DWORD *)m_DibInfo.m_pvData;

	DWORD *ptmpcolor = new DWORD[CFastDIB::m_dwMaxWidth];
	DWORD dwTmpSrc = 0;
	BYTE A=0,R=0,G=0,B=0;
	for (int j=0; j<m_DibInfo.m_bmHeight; j++)
	{
		for (int i=0; i<m_DibInfo.m_bmWidth; i++)
		{
			__asm{
				mov EAX, pSrc
					mov EAX, [EAX]
				mov B, AL
					mov G, AH
					shr EAX, 16
					mov R, AL
					mov A, AH
			} 
			if (pppm->m_bNoise)
			{
				R = rand()%255;
				G = rand()%255;
				B = rand()%255;
				__asm{
					mov AL, R
						mov BH, G
						mov BL, B
						shl EAX, 16
						mov AX, BX
						mov dwTmpSrc, EAX
				}
			}
			else
				dwTmpSrc = *pSrc;

			//计算马赛克效果
			if ((pppm->m_nMosaicHorizontal>0)&&(pppm->m_nMosaicVertical>0))
			{
				if (j%pppm->m_nMosaicVertical==0)
				{
					if (i%pppm->m_nMosaicHorizontal==0)
						ptmpcolor[i] = dwTmpSrc;
					else
						ptmpcolor[i] = ptmpcolor[i-1];
				}
				dwTmpSrc = ptmpcolor[i];
				__asm{
					mov EAX, dwTmpSrc
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL
				}
			}

			if (pppm->m_bReverse)
			{
				__asm{
					mov EAX, dwTmpSrc
						xor EAX, 0xFFFFFFFF
						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL
				}
			}


			if (pppm->m_nLightOffset!=255)
			{
				__int64 submask = 0x00FF00FF00FF00FF;
				__asm{
					pxor mm7, mm7
						movq mm2, submask
						xor EBX, EBX
						mov EAX, pppm
						mov BX, [EAX]PAINTPARAM.m_nLightOffset
						movd mm0, EBX
						movd mm1, dwTmpSrc
						punpckldq mm0, mm0
						punpcklbw mm1, mm7
						movq mm3, mm0
						psllq mm3, 16
						por mm0, mm3
						psubsw mm0, submask
						paddsw mm0, mm1
						packuswb mm0, mm7
						movd EAX, mm0

						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL

						EMMS

				}
			} 

			if (pppm->m_nLightPlus!=64)
			{
				__asm{
					pxor mm7, mm7
						mov EAX, pppm
						movzx EAX, [EAX]PAINTPARAM.m_nLightPlus
						movd mm0, EAX
						movd mm1, dwTmpSrc
						punpckldq mm0, mm0
						punpcklbw mm1, mm7
						movq mm3, mm0
						psllq mm3, 16
						por mm0, mm3
						pmullw mm1, mm0
						psrlw mm1, 6
						packuswb mm1, mm7
						movd EAX, mm1

						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL

						EMMS
				}
			}


			if (pppm->m_nColorPlus!=64)
			{
				__int64 submask = 0x0040004000400040;
				__asm{
					mov EAX, pppm
						movzx EDX, [EAX]PAINTPARAM.m_nColorPlus
						pxor mm7, mm7
						movzx AX, R
						movzx BX, G
						add AX, BX
						movzx BX, B
						add AX, BX
						mov BL, 3
						div BL
						mov ECX, 0
						mov CL, AL
						movd mm0, ECX; mm0<-(R+G+B)/3 "colorAV"
						movd mm1, dwTmpSrc
						punpckldq mm0, mm0
						movq mm4, mm0
						psllq mm4, 16
						por mm0, mm4

						movd mm3, EDX
						punpckldq mm3, mm3
						movq mm4, mm3
						psllq mm4, 16
						por mm3, mm4; mm4<-PAINTPARAM.m_nColorPlus

						punpcklbw mm1, mm7
						movq mm5, mm1; mm5<-A,R,G,B
						psubsw mm1, mm0; mm1<-(R-colorAV), (G-colorAV), (B-colorAV)

						psubsw mm3, submask
						pmullw mm3, mm1
						psraw mm3, 6
						paddsw mm3, mm5
						packuswb mm3, mm7

						movd EAX, mm3
						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL

						EMMS

				}

				/*
				int tmpint = 0;
				int colorAV = (R+G+B)/3;
				short rX = (R-colorAV);
				short gX = (G-colorAV);
				short bX = (B-colorAV);
				tmpint = (int)(R+((pppm->m_nColorPlus-255)*rX>>8));
				if (tmpint>255) tmpint=255;
				else
				if (tmpint<0) tmpint=0;
				R = (BYTE)tmpint;
				tmpint = (int)(G+((pppm->m_nColorPlus-255)*gX>>8));
				if (tmpint>255) tmpint=255;
				else
				if (tmpint<0) tmpint=0;
				G = (BYTE)tmpint;
				tmpint = (int)(B+((pppm->m_nColorPlus-255)*bX>>8));
				if (tmpint>255) tmpint=255;
				else
				if (tmpint<0) tmpint=0;
				B = (BYTE)tmpint;
				dwTmpSrc = (((((A<<8)|R)<<8)|G)<<8)|B;
				A = (BYTE)((dwTmpSrc&0xFF000000)>>24);
				R = (BYTE)((dwTmpSrc&0x00FF0000)>>16);
				G = (BYTE)((dwTmpSrc&0x0000FF00)>>8);
				B = (BYTE)(dwTmpSrc&0x000000FF);
				*/
			}
			if (pppm->m_bColorMask)
			{
				__int64 colormask = 0x00BF00BF00BF00BF;
				__asm{
					pxor mm7, mm7
						movzx AX, R
						movzx BX, G
						add AX, BX
						movzx BX, B
						add AX, BX
						mov BL, 3
						div BL
						mov EBX, 0
						mov BL, AL
						movd mm0, EBX; mm0<-(R+G+B)/3
						mov EAX, pppm
						mov EAX, [EAX]PAINTPARAM.m_dwColorMask
						movd mm1, EAX
						punpckldq mm0, mm0
						movq mm3, mm0
						psllq mm3, 16
						por mm0, mm3
						punpcklbw mm1, mm7
						pand mm1, colormask
						paddusw mm0, mm1
						packuswb mm0, mm7
						movd EAX, mm0
						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL
						EMMS
				}
				/*
				int tmpint = 0;
				BYTE colorAV = (R+G+B)/3;
				tmpint = ((pppm->m_dwColorMask&0x00BF0000)>>16)+colorAV;
				if (tmpint>255) tmpint = 255;
				if (tmpint<0) tmpint = 0;
				R = tmpint;
				tmpint = ((pppm->m_dwColorMask&0x0000BF00)>>8)+colorAV;
				if (tmpint>255) tmpint = 255;
				if (tmpint<0) tmpint = 0;
				G = tmpint;
				tmpint = ((pppm->m_dwColorMask&0x000000BF))+colorAV;
				if (tmpint>255) tmpint = 255;
				if (tmpint<0) tmpint = 0;
				B = tmpint;
				dwTmpSrc = (((((A<<8)|R)<<8)|G)<<8)|B;
				*/
			}

			if (pppm->m_nColorRange>0)
			{
				__asm{
					pxor mm7, mm7
						mov EBX, 0
						mov EAX, pppm
						mov BL, [EAX]PAINTPARAM.m_nColorRange;
					movd mm0, EBX
						movd mm1, dwTmpSrc
						punpcklbw mm1, mm7
						psrlw mm1, mm0
						psllw mm1, mm0
						packuswb mm1, mm7
						movd EAX, mm1
						mov dwTmpSrc, EAX
						mov B, AL
						mov G, AH
						shr EAX, 16
						mov R, AL
						EMMS
				}
			}
			//#ifndef _SATURATION255

			//#endif

			if (pppm->m_nColorOffset != 255)
			{
				BYTE nMax = R;
				int tmpint = 0;
				if (G>nMax) nMax = G;
				if (B>nMax) nMax = B;
				if (R==nMax)
				{
					tmpint = G + ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					G = tmpint;
					tmpint = R - ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					R = tmpint;
				}
				if (G==nMax)
				{
					tmpint = B + ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					B = tmpint;
					tmpint = G - ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					G = tmpint;
				}
				if (B==nMax)
				{
					tmpint = R + ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					R = tmpint;
					tmpint = B - ((pppm->m_nColorOffset-255));
					_SATURATION255(tmpint);
					B = tmpint;
				}
				dwTmpSrc = (((((A<<8)|R)<<8)|G)<<8)|B;

			}

			///////////////////////////////////////
			*pDst = (dwTmpSrc&0x00FFFFFF)|(A<<24);

			pSrc++;
			pDst++;
		}
	}
	delete []ptmpcolor;
	return S_OK;
}

HRESULT CFastDIB::ClearImage()
{
	FASTDIBTYPE tmpType = m_DibInfo.m_nType&0x0000FFFF;
	switch (tmpType)
	{
	case FDIBTYPE_8BITBUF:
		ZeroMemory(m_DibInfo.m_pvData, m_DibInfo.m_bmWidth*m_DibInfo.m_bmHeight);
		break;
	case FDIBTYPE_16BITBUF:
		ZeroMemory(m_DibInfo.m_pvData, m_DibInfo.m_bmWidth*m_DibInfo.m_bmHeight*2);
		break;
	case FDIBTYPE_RGBA:
		ZeroMemory(m_DibInfo.m_pvData, m_DibInfo.m_bmWidth*m_DibInfo.m_bmHeight*4);
		break;
	case FDIBTYPE_DOUBLEBUF:
		ZeroMemory(m_DibInfo.m_pvData, CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight*4);
		ZeroMemory(m_DibInfoBack.m_pvData, CFastDIB::m_dwMaxWidth*CFastDIB::m_dwMaxHeight*4);
		break;
	}
	return S_OK;
}

HRESULT CFastDIB::Fade(BYTE nthreshold)
{
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_DOUBLEBUF, TCM_BELONG);
	DWORD *pSrc = (DWORD *)m_DibInfoBack.m_pvData;
	DWORD *pDst = (DWORD *)m_DibInfo.m_pvData;
	for (int j=0; j<(int)CFastDIB::m_dwMaxHeight; j++)
	{
		for (int i=0; i<(int)CFastDIB::m_dwMaxWidth; i++)
		{
			__asm{
				mov ECX, pSrc
					mov EDX, [ECX]
				mov EAX, EDX
					shr EAX, 24
					mov BL, nthreshold
					mul BL
					shr EAX, 8
					shl EAX, 24
					and EDX, 0x00FFFFFF
					or EDX, EAX
					mov ECX, pDst
					mov [ECX], EDX
			}
			pSrc++;
			pDst++;
		}
	}
	return S_OK;
}

HRESULT CFastDIB::InitPaintParam(PAINTPARAM *pppm)
{
	if (pppm == NULL)
		return E_EMPTYOBJECT;

	ZeroMemory(pppm, sizeof(PAINTPARAM));
	pppm->m_nColorOffset = 255;
	pppm->m_nColorPlus = 64;
	pppm->m_nLightOffset = 255;
	pppm->m_nLightPlus = 64;

	return S_OK;
}

FASTDIB_COLOR CFastDIB::YUVTORGB(FASTDIB_COLOR paramYUV)
{
	/*
	R = Y+1.14V
	G = Y-0.396U-0.581V
	B = Y+2.029U
	*/
	FASTDIB_COLOR rc;
	rc.dw = 0;
	if ((paramYUV.dw & 0x00FFFFFF)==YUV_BLACK )
	{
		rc.dw = 0;
		return rc;
	};

	if ((paramYUV.dw & 0x00FFFFFF)==YUV_WHITE )
	{
		rc.dw = 0;
		return rc;
	};

	int sYUVtemp[3] = {0, 0, 0};
	sYUVtemp[0]= paramYUV.Y - 16;
	sYUVtemp[1]= paramYUV.U - 128;
	sYUVtemp[2]= paramYUV.V - 128;
	int sign[3] = {0, 0, 0};
	for (int i=0; i<3; i++)
	{
		if((sYUVtemp[i] >> 15)>0)
		{
			sign[i]= -1;
			sYUVtemp[i]=(~sYUVtemp[i]) + 1;
		}
		else
			sign[i]=1;
	};

	int R = (sign[0]*sYUVtemp[0]*YUV11-sign[1]*sYUVtemp[1]*YUV21
		+sign[2]*sYUVtemp[2]*YUV31-(1<<15)) >> 16;

	int G = (sign[0]*sYUVtemp[0]*YUV12-sign[1]*sYUVtemp[1]*YUV22
		-sign[2]*sYUVtemp[2]*YUV32-(1<<15)) >> 16;

	int B = (sign[0]*sYUVtemp[0]*YUV13+sign[1]*sYUVtemp[1]*YUV23
		-sign[2]*sYUVtemp[2]*YUV33-(1<<15)) >> 16;

	// limit the RGB value to 0-255
	_SATURATION(0, 255, R);
	_SATURATION(0, 255, G);
	_SATURATION(0, 255, B);

	rc.R = R;
	rc.G = G;
	rc.B = B;

	return rc;
}

FASTDIB_COLOR CFastDIB::RGBTOYUV(FASTDIB_COLOR paramRGB)
{
	/*
	Y = 0.299R+0.587G+0.114B
	U = -0.147R-0.289G+0.436B
	V = 0.615R-0.515G-0.1B
	*/
	FASTDIB_COLOR rc;
	rc.dw = 0;
	if ((paramRGB.dw & 0x00FFFFFF)==RGB_BLACK)
	{
		rc.dw=YUV_BLACK;
		rc.T=paramRGB.A;
		return rc;
	};

	if ((paramRGB.dw & 0x00FFFFFF)==RGB_WHITE)
	{
		rc.dw=YUV_WHITE;
		rc.T=paramRGB.A;
		return rc;
	};

	int R = paramRGB.R;
	int G = paramRGB.G;
	int B = paramRGB.B;
	int T = 0;
	int Y = (((R*RGB11+G*RGB21+B*RGB31+(1<<7))>>8)+(16<<8)+(1<<7))>>8;
	int U = (((B*RGB32-R*RGB12-G*RGB22+(1<<7))>>8)+(128<<8)+(1<<7))>>8;
	int V = (((R*RGB13-G*RGB23-B*RGB33+(1<<7))>>8)+(128<<8)+(1<<7))>>8;

	_SATURATION(16, 235, Y);
	_SATURATION(16, 240, U);
	_SATURATION(16, 240, V);

	rc.Y = Y;
	rc.U = U;
	rc.V = V;

	return rc;
}

HRESULT CFastDIB::FlipBuffer()
{
	_CHECKTYPE(GetDibInfo(), FDIBTYPE_DOUBLEBUF, TCM_BELONG);
	void *p = m_DibInfo.m_pvData; 
	m_DibInfo.m_pvData = m_DibInfoBack.m_pvData;
	m_DibInfoBack.m_pvData = p;
	return S_OK;
}

