// TWainApp.h : main header file for the TWAINAPP application
//

#if !defined(AFX_TWAINAPP_H__DA1E9794_7E6D_4272_8512_499BE16D6087__INCLUDED_)
#define AFX_TWAINAPP_H__DA1E9794_7E6D_4272_8512_499BE16D6087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include <map>

/////////////////////////////////////////////////////////////////////////////
// CTWainAppApp:
// See TWainApp.cpp for the implementation of this class
//

class CTWainAppApp : public CWinApp
{
public:
	CTWainAppApp();

	BOOL ParseCmdLine(CString strCmd, std::map<CString, CString>& mapCmdLine);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTWainAppApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTWainAppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWAINAPP_H__DA1E9794_7E6D_4272_8512_499BE16D6087__INCLUDED_)
