// TWainWrapper.h: interface for the CTWainWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWAINWRAPPER_H__AAA56660_13B5_4CE9_ABA7_A6490DA6BF7D__INCLUDED_)
#define AFX_TWAINWRAPPER_H__AAA56660_13B5_4CE9_ABA7_A6490DA6BF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TWainHelper.h"

#define PM_XFERDONE WM_USER + 1250

class CTWainWrapper  
{
public:
	CTWainWrapper(HWND hWnd = NULL);
	virtual ~CTWainWrapper();

	//初始化和结束化操作
	BOOL InitTwain(HWND hWnd);
	void ReleaseTwain();

	void GetAppIdentity();
	TW_IDENTITY GetSourceIdentity() const { return m_SourceIdentity; };

	//原子操作方法
	BOOL OpenDSM();
	void CloseDSM();

	BOOL SelectDefaultSource();
	BOOL SelectSource();
	BOOL OpenSource(TW_IDENTITY *pSource = NULL);
	void CloseSource();
	
	BOOL GetCapability(TW_CAPABILITY& twCap, TW_UINT16 cap, TW_UINT16 conType = TWON_DONTCARE16);
	BOOL GetCapability(TW_UINT16 cap, TW_UINT32& value);
	BOOL SetCapability(TW_UINT16 cap, TW_UINT16 value, BOOL sign=FALSE);
	BOOL SetCapability(TW_CAPABILITY& twCap);

	BOOL SetImageCount(TW_INT16 nCount = 1);

	BOOL EnableSource(BOOL bShowUI = TRUE);
	BOOL DisableSource();

	BOOL EnableAutofeed();

	BOOL ProcessMessage(MSG msg);
	void TranslateMessage(TW_EVENT& twEvent);
	void TransferImage();

	void CancelTransfer();
	
	void DoMemTransfer();
	void DoFileTransfer();
	void DoNativeTransfer();

	//流程操作方法
	BOOL Acquire(int nCount = 1);

	//Setter & Getter
	BOOL IsValidDriver();
	
	BOOL IsDSMOpen() { return IsValidDriver() && m_bDSMOpen; };
	BOOL IsDSOpen() { return IsValidDriver() && IsDSMOpen() && m_bDSOpen; };
	BOOL IsDSEnable() { return m_bDSEnable; };
	BOOL IsShowUI() { return m_bShowUI; };

	void SetShowUI(BOOL bShow) { m_bShowUI = bShow; };

	TW_INT16 GetRC() const { return m_returnCode; };
	TW_STATUS GetStatus() const { return m_Status; };

protected:
	BOOL LoadTWainLibarary();

private:
	BOOL CallTwainProc(pTW_IDENTITY pOrigin, pTW_IDENTITY pDest, 
						TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG, 
						TW_MEMREF pData);

private:
	//从TWAIN_32.DLL中导出方法
	HINSTANCE m_hTwainDLL;
	DSMENTRYPROC m_lpDSM_Entry;

	//应用程序和源的标识
	TW_IDENTITY m_AppIdentity;
	TW_IDENTITY m_SourceIdentity;

	//窗口句柄
	HWND m_hMessageWnd;

	TW_INT16 m_returnCode;
	TW_STATUS m_Status;
	
	//状态标志
	BOOL m_bDSMOpen;
	BOOL m_bDSOpen;
	BOOL m_bDSEnable;
	BOOL m_bShowUI;

	BOOL m_bAutofeedEnable;

	int m_nImageCount;
};

#endif // !defined(AFX_TWAINWRAPPER_H__AAA56660_13B5_4CE9_ABA7_A6490DA6BF7D__INCLUDED_)
