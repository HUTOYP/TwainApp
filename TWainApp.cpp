// TWainApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TWainApp.h"
#include "TWainAppDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CTWainAppApp

BEGIN_MESSAGE_MAP(CTWainAppApp, CWinApp)
	//{{AFX_MSG_MAP(CTWainAppApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTWainAppApp construction

CTWainAppApp::CTWainAppApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTWainAppApp object

CTWainAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTWainAppApp initialization

BOOL CTWainAppApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CString cszCmd;
	cszCmd = GetCommandLine();

	CTWainHelper::LogMessage("参数列表：");
	CTWainHelper::LogMessage((char*)((LPCTSTR)cszCmd));
	CTWainHelper::LogMessage("\r\n");

	std::map<CString, CString> mapCmdLine;
	if(!ParseCmdLine(cszCmd, mapCmdLine))
	{
		return FALSE;
	}

	CTWainAppDlg dlg;
	m_pMainWnd = &dlg;

	dlg.SetParameter(mapCmdLine);

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

BOOL CTWainAppApp::ParseCmdLine(CString strCmd, std::map<CString, CString>& mapCmdLine)
{
	int index = 0, index_inter = 0;
	CString str_temp, str_key, str_value;

	do 
	{
		strCmd.TrimLeft();
		strCmd.TrimRight();
		strCmd.TrimLeft("\"");
		strCmd.TrimRight("\"");

		index = strCmd.Find(" ");
		if(-1 == index)
		{
			index_inter = strCmd.Find("=");
			if(-1 == index_inter)
				break;
			str_key = strCmd.Left(index_inter);
			str_value = strCmd.Right(strCmd.GetLength() - index_inter - 1);
			
			str_key.TrimLeft();str_key.TrimRight();
			str_value.TrimLeft();str_value.TrimRight();
			
			mapCmdLine.insert(std::map< CString, CString >::value_type(str_key, str_value));

			break;
		}
		
		str_temp = strCmd.Left(index);
		strCmd = strCmd.Right(strCmd.GetLength() - index - 1);

		str_temp.TrimLeft();
		str_temp.TrimRight();

		index_inter = str_temp.Find("=");
		if(-1 == index_inter)
			continue;
		str_key = str_temp.Left(index_inter);
		str_value = str_temp.Right(str_temp.GetLength() - index_inter - 1);

		str_key.TrimLeft();str_key.TrimRight();
		str_value.TrimLeft();str_value.TrimRight();

		mapCmdLine.insert(std::map< CString, CString >::value_type(str_key, str_value));
	
	} while(true);

	return TRUE;
}