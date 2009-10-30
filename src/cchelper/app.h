#pragma once


// DEFINES
//___________________________________________________________________________

#define MEDIAFILE(f)	_T("../../media/"##f)

#define PIECEHV_RJ	(0xD2FC85BF)
#define PIECEHV_RM	(0x17B56D39)
#define PIECEHV_RX	(0xC4F9C567)
#define PIECEHV_RS	(0x826AA68B)
#define PIECEHV_RK	(0x18233962)
#define PIECEHV_RP	(0x402355D8)
#define PIECEHV_RB	(0x03A331A5)

#define PIECEHV_BJ	(0x56FC44E8)
#define PIECEHV_BM	(0x5699E66C)
#define PIECEHV_BX	(0x7AA76288)
#define PIECEHV_BS	(0x2BC9BFCC)
#define PIECEHV_BK	(0x651E8CBB)
#define PIECEHV_BP	(0x737E5B02)
#define PIECEHV_BB	(0x9291F591)

#define PIECE_W		(18)
#define PIECE_H		(18)
#define PIECE_DW	(24)
#define PIECE_DH	(24)


#define PIECE_NUM	(14)

#define MAX_LOADSTRING	256


// STRUCTS
//___________________________________________________________________________

struct PieceStruct
{
	char		cPiece;
	DWORD		dwPieceHashValue;
	TCHAR		* szPieceFile;
	CFastDIB	* pDib;
};

// DECLARATIONS OF FUNCTIONS
//___________________________________________________________________________
BOOL InitApp();
BOOL AppLoop();
BOOL ExitApp();
