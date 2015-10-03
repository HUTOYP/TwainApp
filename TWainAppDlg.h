// TWainAppDlg.h : header file
//

#if !defined(AFX_TWAINAPPDLG_H__35C99712_D625_48EB_B88E_CCFCA2A7178C__INCLUDED_)
#define AFX_TWAINAPPDLG_H__35C99712_D625_48EB_B88E_CCFCA2A7178C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TWainWrapper.h"

#include "pdflib.h"
#include "ximage.h"

#define ULONG_PTR ULONG
#include "GdiPlus.h"

using namespace Gdiplus;

#include <vector>
#include <map>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CTWainAppDlg dialog

class CTWainAppDlg : public CDialog
{
// Construction
public:
	CTWainAppDlg(CWnd* pParent = NULL);	// standard constructor

	void SetParameter(std::map<CString, CString> mapCmdLine);

	HANDLE GetImageHandle(int nIndex){ return m_pic_handles[nIndex]; }

	void RotatePicture(int nImageIndex, int nRotateValue);
	void IsSaveRotate(int nImageIndex, CxImage cxImage);

	static DWORD WINAPI WaitProc(LPVOID lpParameter)
	{
		::MessageBox((HWND)lpParameter, "正在处理，请稍等...", "提示", MB_OK);
		return 0;
	};

// Dialog Data
	//{{AFX_DATA(CTWainAppDlg)
	enum { IDD = IDD_TWAINAPP_DIALOG };
	CButton	m_btn_inscan;
	CButton	m_btn_scan;
	CButton	m_btn_qb;
	CButton	m_btn_jp;
	CButton	m_btn_qhb;
	CStatic	m_static_image;
	CListCtrl	m_ctrl_imagelist;
	CButton	m_btn_right;
	CButton	m_btn_delete;
	CButton	m_btn_left;
	CEdit	m_edit_resolution;
	CEdit	m_edit_rotation;
	CSpinButtonCtrl	m_spin_rotation;
	CComboBox	m_combo_retation;
	CComboBox	m_combo_color;
	CButton	m_btn_movedown;
	CButton	m_btn_moveup;
	CComboBox	m_combo_page;
	CEdit	m_edit_sourcename;
	BOOL	m_showUI;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTWainAppDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	void HandleImage(HANDLE image);

	BOOL GetCurrentSourceCapability();
	BOOL SetCurrentSourceCapability();

// Implementation
protected:
	HICON m_hIcon;

	//获取/设置色彩模式信息
	void GetCapability_PixelType();
	BOOL SetCapability_PixelType(TW_UINT16 value);
	//获取/设置分辨率信息
	void GetCapability_Resolution();
	BOOL SetCapabiliry_Resolution(TW_UINT16 value);
	//获取是否具有双面扫描能力
	BOOL GetCapability_Duplex();
	BOOL SetCapability_Duplex(BOOL isDuplex);
	//获取/设置是否具有旋转角度能力
	void GetCapability_Rotation();
	BOOL SetCapability_Rotation(TW_UINT16 value);

	// Generated message map functions
	//{{AFX_MSG(CTWainAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonScan();
	afx_msg void OnButtonCancle();
	afx_msg void OnSelchangeListImage();
	afx_msg void OnDblclkListImage();
	afx_msg void OnButtonInscan();
	afx_msg void OnButtonSave();
	afx_msg void OnDeltaposSpinRotation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditRotation();
	afx_msg void OnUpdateEditRotation();
	afx_msg void OnClose();
	afx_msg void OnItemchangedListImage(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonLeft90();
	afx_msg void OnButtonRight90();
	afx_msg void OnButtonMoveup();
	afx_msg void OnButtonMovedown();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonQhb();
	afx_msg void OnButtonJp();
	afx_msg void OnButtonQb();
	afx_msg void OnButtonClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// 计算缩略图尺寸
	//void GetProportionSize(int nSrcWidth, int nSrcHeight, int nMaxWidth, int nMaxHeight, int* nNewWidth, int* nNewHeight);
	// 更新CListCtrl中的图片显示
	void UpdateImageListCtrl();
	// 更新m_pic_handles数组中的图片数据
	void UpdateImageAt(int nIndex, HANDLE image)
	{
		int i = 0;
		std::vector< HANDLE >::iterator it;
		for(it = m_pic_handles.begin(); it != m_pic_handles.end(); it++)
		{
			if(nIndex != i)
			{
				i++;
				continue;
			}
			
			*it = image;
			break;
		}
	}

private:
	CTWainWrapper* m_twainWrapper;
	std::map< CString, CString > m_map_cmdline;

	std::vector< HANDLE > m_pic_handles;
	CBitmap m_bmp;

	//插扫标志
	BOOL m_flag_insertscan;

	int m_current_color;
	int m_current_rotation;
	int m_current_resolution;

	int m_minResolution;
	int m_maxResolution;

	CString m_strExePath;

	int m_previous_rotatevalue;

private:

	//保存图片列表
	CImageList m_imageList;
	//位图对象
	//CBitmap m_bitmap;
	
	CToolTipCtrl m_toolTip;
	
	BOOL m_initPictureCtrl;
	CxImage* m_pTempCxImage;
	
	ULONG_PTR m_gdiplusToken;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWAINAPPDLG_H__35C99712_D625_48EB_B88E_CCFCA2A7178C__INCLUDED_)
