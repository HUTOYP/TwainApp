// TWainHelper.h: interface for the CTWainHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWAINHELPER_H__4708E34F_3FEF_48A6_AF07_51E7C0E70261__INCLUDED_)
#define AFX_TWAINHELPER_H__4708E34F_3FEF_48A6_AF07_51E7C0E70261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "twain.h"

#include <vector>

#define LOGSIZE 1024

#define MESSAGEBOX_TITLE_ERROR "错误提示"
#define MESSAGEBOX_TITLE_INFO "提示信息"

typedef struct
{
	const char* pszItemName;
	TW_UINT16 ItemId;
} TABLEENTRY, *pTABLEENTRY;

#define MAX_SETUPXFERTYPE 3
#define MAX_PIXELTYPE 9

extern TABLEENTRY SetupXferType[MAX_SETUPXFERTYPE];
extern TABLEENTRY PixelType[MAX_PIXELTYPE];

/////////////////////////////////////////////////////////////////////

class CTWainHelper  
{
public:
	static void LogMessage(char msg[]);
	static BOOL SaveBMP(HGLOBAL _hDIB, char** picData, int* picLen);

	static BOOL SaveAsPdf(const char* pFileName, std::vector< HANDLE > m_pic_handles);
	static int Upload(const char* pParameter);

	static void GetProportionSize(int nSrcWidth, int nSrcHeight, int nMaxWidth, int nMaxHeight, int* nNewWidth, int* nNewHeight);

};

#endif // !defined(AFX_TWAINHELPER_H__4708E34F_3FEF_48A6_AF07_51E7C0E70261__INCLUDED_)
