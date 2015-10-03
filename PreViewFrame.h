// PreViewFrame.h: interface for the PreViewFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREVIEWFRAME_H__C5BD517D_7991_43C2_ACE2_2ECD73A40CF7__INCLUDED_)
#define AFX_PREVIEWFRAME_H__C5BD517D_7991_43C2_ACE2_2ECD73A40CF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define USER_XFERDONE WM_USER + 1024

extern HWND hWnd;
extern BOOL bPreviewing;

BOOL CreatePreViewFrame(HINSTANCE instance);

#endif // !defined(AFX_PREVIEWFRAME_H__C5BD517D_7991_43C2_ACE2_2ECD73A40CF7__INCLUDED_)
