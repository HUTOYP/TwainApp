#if !defined(AFX_ROTATEDIALOG_H__4C2C3B3E_C3D9_4724_8B85_4DB5EEEF9538__INCLUDED_)
#define AFX_ROTATEDIALOG_H__4C2C3B3E_C3D9_4724_8B85_4DB5EEEF9538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateDialog.h : header file
//

#include "TWainAppDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog dialog

class CRotateDialog : public CDialog
{
// Construction
public:
	CRotateDialog(CWnd* pParent = NULL);   // standard constructor

	void SetSelectedIndex(int nSelectedIndex) 
	{
		m_nSelectedIndex = nSelectedIndex; 
	
		HANDLE image = parentDlg->GetImageHandle(m_nSelectedIndex);
		m_cxSelectedImage.CreateFromHANDLE(image);
	}

// Dialog Data
	//{{AFX_DATA(CRotateDialog)
	enum { IDD = IDD_DIALOG_XZ };
	CStatic	m_value;
	CSliderCtrl	m_slider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotateDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotateDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CTWainAppDlg* parentDlg;

	int m_nSelectedIndex;
	CxImage m_cxSelectedImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATEDIALOG_H__4C2C3B3E_C3D9_4724_8B85_4DB5EEEF9538__INCLUDED_)
