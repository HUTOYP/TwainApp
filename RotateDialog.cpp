// RotateDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TWainApp.h"
#include "RotateDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog dialog


CRotateDialog::CRotateDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRotateDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotateDialog)
	//}}AFX_DATA_INIT

	parentDlg = (CTWainAppDlg*)pParent;
}


void CRotateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotateDialog)
	DDX_Control(pDX, IDC_STATIC_VALUE, m_value);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotateDialog, CDialog)
	//{{AFX_MSG_MAP(CRotateDialog)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER, OnReleasedcaptureSlider)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotateDialog message handlers

BOOL CRotateDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_slider.SetTicFreq(5);
	m_slider.SetLineSize(1);

	m_slider.SetRange(-30, 30, TRUE);
	m_slider.SetPos(0);

	m_value.SetWindowText("0");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRotateDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
/*
	int nCurrentSliderValue = m_slider.GetPos();
	CString strCurrentSliderValue = "";
	strCurrentSliderValue.Format("%d", nCurrentSliderValue);

	m_value.SetWindowText(strCurrentSliderValue);

	parentDlg->RotatePicture(m_nSelectedIndex, nCurrentSliderValue);
*/
}

void CRotateDialog::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int nCurrentSliderValue = m_slider.GetPos();
	CString strCurrentSliderValue = "";
	strCurrentSliderValue.Format("%d", nCurrentSliderValue);
	
	m_value.SetWindowText(strCurrentSliderValue);
	
	parentDlg->RotatePicture(m_nSelectedIndex, nCurrentSliderValue);

	*pResult = 0;
}

void CRotateDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	parentDlg->IsSaveRotate(m_nSelectedIndex, m_cxSelectedImage);

	CDialog::OnClose();
}
