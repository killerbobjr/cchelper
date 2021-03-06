#pragma once

#define E_UNSUPORTCOLOR 0x90000001
#define E_LOADIMAGEFAIL 0x90000002
#define E_GETOBJECTFAIL 0x90000003
#define E_FILENOTFOUND 0x90000004
#define E_TYPEMISMATCH 0x90000005
#define E_INVALIDSIZE 0x90000006
#define E_EMPTYOBJECT 0x90000007
#define E_DIBLOCKED 0x90000008
#define E_NOTINIT 0x90000009

#define TM_NONE 0
#define TM_RGB 1
#define TM_ALPHA 2

#define DRAWMODE_IGNOREALPHA 0x0000007F
#define DRAWMODE_FLIPALPHA 0x0000003F
#define DRAWMODE_XADDSATURATION 0x0000001F
#define DRAWMODE_XSUBSATURATION 0x0000000F
#define DRAWMODE_ADDSATURATION 0x00000007
#define DRAWMODE_SUBSATURATION 0x00000003
#define DRAWMODE_NORMAL 0x00000001
#define DRAWMODE_COPY 0x00000000

#define DRAWMODE_FROMBACK 0x00010000
#define DRAWMODE_FLIPSRCCOLOR 0x00100000
#define DRAWMODE_FLIPDSTCOLOR 0x01000000
#define DRAWMODE_ALPHABLEND 0x10000000

#define FDIBTYPE_RGBA 0x00000001
#define FDIBTYPE_8BITBUF 0x00000003
#define FDIBTYPE_16BITBUF 0x00000007
#define FDIBTYPE_DOUBLEBUF 0x0000000F
#define FDIBTYPE_NULLFRONT 0x00010000
#define FDIBTYPE_NULLBACK 0x00020000

typedef UINT FASTDIBTYPE;
#define _COLORREFTOARGB(color, nAlphaValue) DWORD XR = (color&0x000000FF)<<16; DWORD XB = (color&0x00FF0000)>>16; DWORD XA = nAlphaValue<<24; color = (color&0x0000FF00)|XR|XB|XA;
#define _SATURATION(min, max, value) if (value < min) value = min; if (value > max) value = max;




typedef union
{
	DWORD dw;
	struct
	{
		unsigned char V; 
		unsigned char U;
		unsigned char Y;
		unsigned char T; 
	};
	struct
	{
		unsigned char B; 
		unsigned char G;
		unsigned char R;
		unsigned char A;
	};
} FASTDIB_COLOR;

typedef union _FDIBMATRIX
{
	struct _STRUCTFDIBMATRIX
	{
		DWORD part0;
		DWORD part1;
		DWORD part2;
		DWORD part3;
		DWORD part4;
		DWORD part5;
		DWORD part6;
		DWORD part7;
		DWORD part8;
	} sPart;
	DWORD aPart[3][3];
} FDIBMATRIX;

typedef struct tagFASTDIB
{
	FASTDIBTYPE m_nType;
	LONG m_bmWidth;
	LONG m_bmHeight;
	void *m_pvData;
} FASTDIB, *PFASTDIB;

typedef struct tagDRAWPARAM
{
	int nXDst;
	int nYDst;
	int nWidth;
	int nHeight;
	int nXSrc;
	int nYSrc;
	int nDstWidth;
	int nDstHeight;
	int nSrcWidth;
	int nSrcHeight;
	DWORD *pDst;
	DWORD *pDst2;
	DWORD *pSrc;
	DWORD nBoardColor;
	BYTE nAlphaValue;
	BYTE nBoardWidth;
	int nShadowHorPos;
	int nShadowVerPos;
	int nShadowHorSize;
	int nShadowVerSize;
	DWORD nShadowColor;
} DRAWPARAM, *PDRAWPARAM;

typedef struct tagCOLORTABLE
{
	DWORD m_part0;
	DWORD m_part1;
	DWORD m_part2;
	DWORD m_part3;
} COLORTABLE, *PCOLORTABLE;

typedef struct tagANGLETABLE
{
	int m_sinpart;
	int m_cospart;
} ANGLETABLE, *PANGLETABLE;

typedef struct tagCOLORTRANSPARENCY
{
	BOOL m_bColorKey;
	BOOL m_bLightKey;
	WORD m_wHue;
	BYTE m_wHueRange;
	BYTE m_nSaturation;
	BYTE m_nBoardBlur;
	BYTE m_nShadowRange;
	BYTE m_nHighLevelDoor;
	BYTE m_nLowLevelDoor;
	BYTE m_nHighLevelBlur;
	BYTE m_nLowLevelBlur;

	BYTE m_nTransparentValue;
} COLORTRANSPARENCY, *PCOLORTRANSPARENCY;

typedef struct tagPAINTPARAM
{
	BYTE m_nMosaicHorizontal;//水平马赛克
	BYTE m_nMosaicVertical;//垂直马赛克
	BOOL m_bFlipColor;//色彩反转 
	BOOL m_bFlipLight;//亮度反转 
	BOOL m_bReverse;//反相 
	BOOL m_bColorMask;//是否蒙色 
	DWORD m_dwColorMask;//画面蒙色 
	BYTE m_nLightPlus;//亮度增益
	WORD m_nLightOffset;//亮度偏移 
	BYTE m_nColorPlus;//色度增益 
	WORD m_nColorOffset;//色度偏移 
	BYTE m_nLightRange;//亮度分量 
	BYTE m_nColorRange;//色度分量
	BOOL m_bNoise;//杂色 
} PAINTPARAM, *PPAINTPARAM;

#define YUV11 0x129E4
#define YUV12 0x129E4
#define YUV13 0x129E4
#define YUV21 0x1D5
#define YUV22 0x62EB
#define YUV23 0x201DF
#define YUV31 0x19589
#define YUV32 0xCF41
#define YUV33 0x12F

#define RGB11 0x41FF
#define RGB12 0x25FD
#define RGB13 0x70FF
#define RGB21 0x80FF
#define RGB22 0x4B02
#define RGB23 0x5F00
#define RGB31 0x1903
#define RGB32 0x70FF
#define RGB33 0x11FF

#define YUV_BLACK 0x00108080
#define YUV_WHITE 0x00EB8080
#define YUV_LIGHTGRAY 0x00B58080
#define YUV_DARKGRAY 0x00478080
#define RGB_BLACK 0x00000000
#define RGB_WHITE 0x00FFFFFF

class CFastDIB 
{
public:
	CFastDIB();
	virtual ~CFastDIB();
private:
	FASTDIB m_DibInfo;
	FASTDIB m_DibInfoBack;
private:
	static BOOL m_bInited;
	static DWORD m_dwMaxHeight;
	static DWORD m_dwMaxWidth;
	static COLORTABLE m_ct[32][32];
	static ANGLETABLE m_at[3600];
	static DWORD m_hsltable[262144];
	static int *m_mapx;
	static int *m_offsetx;
	BOOL m_bLocked;
	void _DrawCopy(DRAWPARAM *dpm);

	void _DrawIgnoreAlpha(DRAWPARAM *pdpm);
	void _DrawNormal(DRAWPARAM *pdpm);
	void _DrawFlipAlpha(DRAWPARAM *pdpm);

	void _DrawAddSaturation(DRAWPARAM *pdpm);
	void _DrawAddSaturationFlipAlpha(DRAWPARAM *pdpm);

	void _DrawSubSaturation(DRAWPARAM *pdpm);
	void _DrawSubSaturationFlipAlpha(DRAWPARAM *pdpm);

	void _MatrixTrans(FDIBMATRIX *pfdMatrix1, FDIBMATRIX fdMatrix2);
	void _MatrixTrans(POINT *pcpCoord, FDIBMATRIX fdMatrix);

public:
	HRESULT FlipBuffer();
	static FASTDIB_COLOR YUVTORGB(FASTDIB_COLOR paramYUV);
	static FASTDIB_COLOR RGBTOYUV(FASTDIB_COLOR paramRGB);
	static HRESULT InitPaintParam(PAINTPARAM *pppm);
	HRESULT Fade(BYTE nthreshold);
	HRESULT ClearImage();
	static HRESULT Finalization();
	static void Initialize(DWORD dwMaxWidth, DWORD dwMaxHeight);
	HRESULT CreateDIB(DWORD dwWidth, DWORD dwHeight, FASTDIBTYPE fdt);
	HRESULT CreateCompatibleDIB(CFastDIB *pDIB);
	HRESULT ActivateBuf(int x, int y, BYTE nEnerge);
	HRESULT ActivateBuf16(int x, int y, int r, WORD nEnerge);
	HRESULT DistortByDIB(int x, int y, CFastDIB *pDIB);
	HRESULT DistortByDIB16(int x, int y, CFastDIB *pDIB);
	static HRESULT WaterFX(CFastDIB *pCurrentBuf, CFastDIB *pNextBuf);
	static HRESULT WaterFX16(CFastDIB *pCurrentBuf, CFastDIB *pNextBuf);
	static void memcpy2(void *lpvDst, void *lpvSrc, DWORD cpySize);
	static const char * GetErrorString(HRESULT hErrorCode);

	HRESULT Stretch(double fxtime, double fytime, DWORD x, DWORD y, DWORD nWidth, DWORD nHeight, BOOL bBlur = TRUE);
	HRESULT Rotation(int nangle, int cx, int cy, DWORD nWidth, DWORD nHeight, BOOL bBlur = TRUE);
	HRESULT BitmapWipe(CFastDIB *pBufDIB, BYTE nCurrentTime, BYTE nBoardWidth=0, 
		BYTE nBoardWidthColor=0,BOOL bColorBoard=FALSE, 
		DWORD nBoardColor=RGB(0, 0, 0), DWORD x=0, DWORD y=0);
	HRESULT SyncDoubleBuf();
	HRESULT ColorTransparency(PCOLORTRANSPARENCY pctr);
	HRESULT Paint(PPAINTPARAM pppm);
	HRESULT QuickPaint(PPAINTPARAM pppm);
	HRESULT FastBlt(HDC hdc, int nXDst=0, int nYDst=0, int Width=0, int Height=0, int nXSrc=0, int nYSrc=0);
	HRESULT Draw(CFastDIB *pDstDIB, UINT uDrawMode=DRAWMODE_COPY, int nXDst=0, int nYDst=0, int nWidth=0, int nHeight=0, int nXSrc=0, int nYSrc=0,
	BYTE nAlphaValue = 0xFF, BYTE nBoardWidth = 0, DWORD nBoardColor = 0);
	void FreeDIB();
	HRESULT LoadFromDIB(CFastDIB *pSrcDib);
	HRESULT LoadFromDIBEX(CFastDIB *pSrcDib);
	HRESULT LoadFromFile(LPCTSTR lpszFileName);
	HRESULT LoadFromHBitmap(HBITMAP hSrcBitmap);
	HRESULT LoadFromBuf(FASTDIB *lpDibInfo, FASTDIB *lpDibInfoBack);

	FASTDIB *GetDibInfo() { return &m_DibInfo; };
	FASTDIB *GetBackDibInfo() { return &m_DibInfoBack; };
};

