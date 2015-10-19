// TWainWrapper.cpp: implementation of the CTWainWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TWainApp.h"
#include "TWainWrapper.h"

#define LOGSIZE 1024
#define WINDIRPATHSIZE 160	// Windows路径最大长度
#define	VALID_HANDLE 32	// valid windows handle >= 32

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTWainWrapper::CTWainWrapper(HWND hWnd)
{
	m_hTwainDLL = NULL;
	m_lpDSM_Entry = NULL;
	
	memset(&m_AppIdentity, 0, sizeof(TW_IDENTITY));
	memset(&m_SourceIdentity, 0, sizeof(TW_IDENTITY));

	m_hMessageWnd = NULL;

	m_bDSMOpen = FALSE;
	m_bDSOpen = FALSE;
	m_bDSEnable = FALSE;
	m_bShowUI = FALSE;
	m_bAutofeedEnable = FALSE;
	
	m_nImageCount = 0;
	m_returnCode = TWRC_SUCCESS;

	if(hWnd)
	{
		InitTwain(hWnd);
	}
	else
	{
		char logInfo[LOGSIZE];
		sprintf(logInfo, "窗口句柄为空，未能在构造函数中初始化TWain。%s:%d\r\n", __FILE__, __LINE__);
		CTWainHelper::LogMessage(logInfo);
	}
}

CTWainWrapper::~CTWainWrapper()
{

}

BOOL CTWainWrapper::LoadTWainLibarary()
{
	if(IsValidDriver())
		return TRUE;

	//载入动态库和函数
	char WinDir[WINDIRPATHSIZE];
	memset(WinDir, 0, sizeof(char[WINDIRPATHSIZE]));
	
	//获得系统安装路径
	GetWindowsDirectory (WinDir, WINDIRPATHSIZE);
	
	TW_STR32 DSMName;
	memset(DSMName, 0, sizeof(TW_STR32));
	memcpy(DSMName, "TWAIN_32.DLL", strlen("TWAIN_32.DLL"));
	
	if (WinDir[strlen(WinDir)-1] != '\\')
	{
		lstrcat (WinDir, "\\");
	}
	lstrcat (WinDir, DSMName);
	
	OFSTRUCT OpenFiles;
	memset(&OpenFiles, 0, sizeof(OFSTRUCT));
	
	if ((OpenFile(WinDir, &OpenFiles, OF_EXIST) != -1) &&
		(m_hTwainDLL = LoadLibrary(DSMName)) != NULL &&
		(m_hTwainDLL >= (HANDLE)VALID_HANDLE) &&
		(m_lpDSM_Entry = (DSMENTRYPROC)GetProcAddress((HMODULE)m_hTwainDLL, MAKEINTRESOURCE (1))) != NULL)
	{
		CTWainHelper::LogMessage("Load library Success!\r\n");
		return TRUE;
	}
	else
		return FALSE;
}

void CTWainWrapper::GetAppIdentity()
{
	//TWAIN initialization
	m_AppIdentity.Id = 0; 				// init to 0, but Source Manager will assign real value
	m_AppIdentity.Version.MajorNum = 1;
	m_AppIdentity.Version.MinorNum = 0;
	m_AppIdentity.Version.Language = TWLG_CHINESE;
	m_AppIdentity.Version.Country  = TWCY_CHINA;
	
	lstrcpy (m_AppIdentity.Version.Info,  "TWAIN_Win32 App 1.0.0");
	lstrcpy (m_AppIdentity.ProductName,   "TWAINAPP_Win32");
	
	m_AppIdentity.ProtocolMajor = TWON_PROTOCOLMAJOR;
	m_AppIdentity.ProtocolMinor = TWON_PROTOCOLMINOR;
	m_AppIdentity.SupportedGroups =  DG_IMAGE | DG_CONTROL;
	lstrcpy (m_AppIdentity.Manufacturer,  "CECT15");
	lstrcpy (m_AppIdentity.ProductFamily, "CB");
}

BOOL CTWainWrapper::InitTwain(HWND hWnd)
{
	if(!IsWindow(hWnd))
		return FALSE;
	m_hMessageWnd = hWnd;

	//载入TWAIN动态库和函数
	if(!LoadTWainLibarary())
	{
		char logInfo[LOGSIZE];
		sprintf(logInfo, "载入TWAIN动态库失败，TWAIN初始化失败。%s:%d\r\n", __FILE__, __LINE__);
		CTWainHelper::LogMessage(logInfo);
		
		return FALSE;
	}

	return OpenDSM();
}

void CTWainWrapper::ReleaseTwain()
{

}

BOOL CTWainWrapper::OpenDSM()
{
	//构造AppIdentity
	GetAppIdentity();
	
	//打开DSM
	m_bDSMOpen = CallTwainProc(&m_AppIdentity, NULL, 
								DG_CONTROL, DAT_PARENT, MSG_OPENDSM,
								(TW_MEMREF)&m_hMessageWnd);
	return m_bDSMOpen;
}

void CTWainWrapper::CloseDSM()
{
	if(IsDSMOpen())
	{
		CloseSource();
		CallTwainProc(&m_AppIdentity, NULL, 
						DG_CONTROL, DAT_PARENT, MSG_CLOSEDSM, 
						(TW_MEMREF)&m_hMessageWnd);
		m_bDSMOpen = FALSE;
	}
}

BOOL CTWainWrapper::SelectDefaultSource()
{
	return CallTwainProc(&m_AppIdentity, NULL, 
							DG_CONTROL, DAT_IDENTITY, MSG_GETDEFAULT,
							&m_SourceIdentity);
}

BOOL CTWainWrapper::SelectSource()
{
	memset(&m_SourceIdentity, 0, sizeof(TW_IDENTITY));

	return CallTwainProc(&m_AppIdentity, NULL, 
							DG_CONTROL, DAT_IDENTITY, MSG_USERSELECT, 
							&m_SourceIdentity);
}

BOOL CTWainWrapper::OpenSource(TW_IDENTITY *pSource)
{
	if(pSource) 
	{
		m_SourceIdentity = *pSource;
	}

	if(IsDSMOpen())
	{
		m_bDSOpen = CallTwainProc(&m_AppIdentity, NULL, 
									DG_CONTROL, DAT_IDENTITY, MSG_OPENDS, 
									(TW_MEMREF)&m_SourceIdentity);
	}
	return IsDSOpen();
}

void CTWainWrapper::CloseSource()
{
	if(IsDSOpen())
	{
		DisableSource();
		CallTwainProc(&m_AppIdentity, NULL, 
						DG_CONTROL, DAT_IDENTITY, MSG_CLOSEDS, 
						(TW_MEMREF)&m_SourceIdentity);
		m_bDSOpen = FALSE;
	}
}

BOOL CTWainWrapper::GetCapability(TW_CAPABILITY& twCap, TW_UINT16 cap, TW_UINT16 conType)
{
	if(IsDSOpen())
	{
		twCap.Cap = cap;
		twCap.ConType = conType;
		twCap.hContainer = NULL;
		
		if(CallTwainProc(&m_AppIdentity, &m_SourceIdentity,
							DG_CONTROL, DAT_CAPABILITY, MSG_GET, 
							(TW_MEMREF)&twCap))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTWainWrapper::GetCapability(TW_UINT16 cap, TW_UINT32& value)
{
	TW_CAPABILITY twCap;
	if(GetCapability(twCap, cap))
	{
		pTW_ONEVALUE pVal;
		pVal = (pTW_ONEVALUE )GlobalLock(twCap.hContainer);
		if(pVal)
		{
			value = pVal->Item;
			GlobalUnlock(pVal);
			GlobalFree(twCap.hContainer);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTWainWrapper::SetCapability(TW_UINT16 cap,TW_UINT16 value,BOOL sign)
{
	if(IsDSOpen())
	{
		TW_CAPABILITY twCap;
		pTW_ONEVALUE pVal;
		BOOL ret_value = FALSE;
		
		twCap.Cap = cap;
		twCap.ConType = TWON_ONEVALUE;
		twCap.hContainer = GlobalAlloc(GHND, sizeof(TW_ONEVALUE));
		
		if(twCap.hContainer)
		{
			pVal = (pTW_ONEVALUE)GlobalLock(twCap.hContainer);
			pVal->ItemType = sign ? TWTY_INT16 : TWTY_UINT16;
			pVal->Item = (TW_UINT32)value;

			GlobalUnlock(twCap.hContainer);
			ret_value = SetCapability(twCap);
			GlobalFree(twCap.hContainer);
		}
		return ret_value;
	}
	return FALSE;
}

BOOL CTWainWrapper::SetCapability(TW_CAPABILITY& cap)
{
	if(IsDSOpen())
	{
		return CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
								DG_CONTROL, DAT_CAPABILITY, MSG_SET, 
								(TW_MEMREF)&cap);
	}
	return FALSE;
}

BOOL CTWainWrapper::SetImageCount(TW_INT16 nCount)
{
	if(SetCapability(CAP_XFERCOUNT,(TW_UINT16)nCount,TRUE))
	{
		m_nImageCount = nCount;
		return TRUE;
	}
	else
	{
		if(GetRC() == TWRC_CHECKSTATUS)
		{
			TW_UINT32 count;
			if(GetCapability(CAP_XFERCOUNT,count))
			{
				nCount = (TW_INT16)count;
				if(SetCapability(CAP_XFERCOUNT,nCount))
				{
					m_nImageCount = nCount;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CTWainWrapper::EnableSource(BOOL bShowUI)
{
	if(IsDSOpen() && !IsDSEnable())
	{
		TW_USERINTERFACE twUI;
		memset(&twUI, 0, sizeof(TW_USERINTERFACE));

		twUI.hParent = (TW_HANDLE)m_hMessageWnd;
		twUI.ShowUI = bShowUI;
		
		if(CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
							DG_CONTROL, DAT_USERINTERFACE, MSG_ENABLEDS,
							(TW_MEMREF)&twUI))
		{
			m_bDSEnable = TRUE;
			m_bShowUI = bShowUI;
		}
		else
		{
			m_bDSEnable = FALSE;
			m_bShowUI = FALSE;
		}
		return m_bDSEnable;
	}
	return FALSE;
}

BOOL CTWainWrapper::DisableSource()
{
	if(IsDSEnable())
	{
		TW_USERINTERFACE twUI;
		memset(&twUI, 0, sizeof(TW_USERINTERFACE));
		
		twUI.hParent = (TW_HANDLE)m_hMessageWnd;
		twUI.ShowUI = TWON_DONTCARE8;

		if(CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
							DG_CONTROL, DAT_USERINTERFACE, MSG_DISABLEDS, 
							&twUI))
		{
			m_bDSEnable = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CTWainWrapper::ProcessMessage(MSG msg)
{
	if(IsDSEnable())
	{
		TW_EVENT twEvent;
		twEvent.pEvent = (TW_MEMREF)&msg;
		twEvent.TWMessage = MSG_NULL;
	
		CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
						DG_CONTROL, DAT_EVENT, MSG_PROCESSEVENT, 
						(TW_MEMREF)&twEvent);
		if(GetRC() != TWRC_NOTDSEVENT)
		{
			TranslateMessage(twEvent);
		}
		return FALSE;

	}
	return FALSE;
}

void CTWainWrapper::TranslateMessage(TW_EVENT& twEvent)
{
	switch(twEvent.TWMessage)
	{
	case MSG_XFERREADY:
		TransferImage();
		break;
	case MSG_CLOSEDSREQ:
	case MSG_CLOSEDSOK:
		CloseSource();
		break;
	default:
		break;
	}
}

void CTWainWrapper::TransferImage()
{
	TW_CAPABILITY twCapability;
	pTW_ENUMERATION pEnumeration = NULL;		
	TW_UINT16 index = 0, i = 0;
	TW_STR64 buffer;
	TW_UINT16 ModeType = 0;

	memset(buffer, 0, sizeof(TW_STR64));		
	memset(&twCapability, 0, sizeof(TW_CAPABILITY));
	twCapability.Cap = ICAP_XFERMECH;

	/*
	* Call the triplet with MSG_GET to get the current Value of 
	* ICAP_XFERMECH
	*/
	CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
					DG_CONTROL, DAT_CAPABILITY, MSG_GET, 
					(TW_MEMREF)&twCapability);

	pEnumeration = (pTW_ENUMERATION)GlobalLock(twCapability.hContainer);

	for (index = 0;index < pEnumeration->NumItems; index++)
	{
		if(index == pEnumeration->CurrentIndex)
		{
			for(i = 0; i < MAX_SETUPXFERTYPE; i++)
			{
				if(*(TW_UINT16*)&pEnumeration->ItemList[index*(sizeof(TW_UINT16))] == SetupXferType[i].ItemId)
				{
					TRACE("Value = %s.\n", SetupXferType[i].pszItemName);
					ModeType = SetupXferType[i].ItemId;
					break;
				}
			}
		}
	}
	GlobalUnlock(twCapability.hContainer);
	GlobalFree(twCapability.hContainer);

	if(ModeType == TWSX_MEMORY)
	{
		DoMemTransfer();
	}
	else if(ModeType == TWSX_FILE)
	{
		DoFileTransfer();
	}
	else if(ModeType == TWSX_NATIVE)
	{
		DoNativeTransfer();
	}
}

void CTWainWrapper::DoMemTransfer()
{
}

void CTWainWrapper::DoFileTransfer()
{
	//文件模式适用于大图片，数量较多的情况
}

void CTWainWrapper::DoNativeTransfer()
{
	TW_PENDINGXFERS     twPendingXfer;
	TW_UINT16           twRC = TWRC_FAILURE;
	TW_UINT16           twRC2 = TWRC_FAILURE;
	TW_UINT32           hBitMap = NULL;
	HANDLE              hbm_acq = NULL;     // handle to bit map from Source to ret to App
	char buffer[2048];

	LPBITMAPINFOHEADER lpdib = NULL;

	memset(&twPendingXfer, 0, sizeof(TW_PENDINGXFERS));
	memset(buffer, 0, sizeof(char[2048]));

	/*
	* Do until there are no more pending transfers
	* explicitly initialize the our flags
	*/
	twPendingXfer.Count = 0;
	do 
	{
		/*
		* Initiate Native Transfer
		*/
		twRC = m_lpDSM_Entry(&m_AppIdentity, &m_SourceIdentity, 
							DG_IMAGE, DAT_IMAGENATIVEXFER, MSG_GET, 
							(TW_MEMREF)&hBitMap);

		switch (twRC)
		{
			case TWRC_XFERDONE:  // Session is in State 7
            
				hbm_acq = (HBITMAP)hBitMap;

				/*
				* Acknowledge the end of the transfer 
				* and transition to state 6/5
				*/
				twRC2 = m_lpDSM_Entry(&m_AppIdentity, &m_SourceIdentity, 
										DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER, 
										(TW_MEMREF)&twPendingXfer);

				if (twRC2 != TWRC_SUCCESS)
				{
					CTWainHelper::LogMessage("Close transfer failure!\r\n");
				}

				wsprintf(buffer,"Pending Xfers = %d\r\n",twPendingXfer.Count);
				CTWainHelper::LogMessage(buffer);

				if (twPendingXfer.Count == 0)
				{	
					if (hbm_acq && (lpdib = (LPBITMAPINFOHEADER)GlobalLock(hbm_acq))!=NULL)
					{
						if(!m_bShowUI)
						{
							CloseSource();
						}
						GlobalUnlock(hbm_acq);
					}
				}

				if (hbm_acq >= (HANDLE)VALID_HANDLE)
				{
					SendMessage(m_hMessageWnd, PM_XFERDONE, (WPARAM)hbm_acq, 0);
				}
				else
				{
					SendMessage(m_hMessageWnd, PM_XFERDONE, NULL, 0);
				}
				break;

			/*
			* the user canceled or wants to rescan the image
			* something wrong, abort the transfer and delete the image
			* pass a null ptr back to App
			*/
			case TWRC_CANCEL:   // Session is in State 7
				/*
				* Source (or User) Canceled Transfer
				* transistion to state 6/5
				*/
				twRC2 = m_lpDSM_Entry(&m_AppIdentity, &m_SourceIdentity, 
									DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);

				if (twRC2 != TWRC_SUCCESS)
				{
					CTWainHelper::LogMessage("Close transfer failure!\r\n");
				}

				if (twPendingXfer.Count == 0)
				{
					if(!m_bShowUI)
					{
						CloseSource();
					}
					CTWainHelper::LogMessage("twPendingXfer.Count == 0!\r\n");
				}

				SendMessage(m_hMessageWnd, PM_XFERDONE, NULL, 0);
				break;

			case TWRC_FAILURE:  //Session is in State 6
				/*
				* The transfer failed
				*/
				CTWainHelper::LogMessage("The transfer failed!\r\n");

				/*
				* Abort the image
				* Enhancement: Check Condition Code and attempt recovery
				*/
				twRC2 = m_lpDSM_Entry(&m_AppIdentity, &m_SourceIdentity, 
									DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);

				if (twRC2 != TWRC_SUCCESS)
				{
					CTWainHelper::LogMessage("Close transfer failure!\r\n");
				}

				if (twPendingXfer.Count == 0)
				{
					if(!m_bShowUI)
					{
						CloseSource();
					}
					CTWainHelper::LogMessage("twPendingXfer.Count == 0!\r\n");
				}

				SendMessage(m_hMessageWnd, PM_XFERDONE, NULL, 0);
				break;

			default:    //Sources should never return any other RC
				/*
				* Abort the image
				* Enhancement: Check Condition Code and attempt recovery instead
				*/
				twRC2 = m_lpDSM_Entry(&m_AppIdentity, &m_SourceIdentity, 
									DG_CONTROL, DAT_PENDINGXFERS, MSG_ENDXFER,
									(TW_MEMREF)&twPendingXfer);

				if (twRC2 != TWRC_SUCCESS)
				{
					CTWainHelper::LogMessage("Close transfer failure!\r\n");
				}
				
				if (twPendingXfer.Count == 0)
				{
					if(!m_bShowUI)
					{
						CloseSource();
					}
					CTWainHelper::LogMessage("twPendingXfer.Count == 0!\r\n");
				}

				SendMessage(m_hMessageWnd, PM_XFERDONE, NULL, 0);
				break;
		}   

	} while (twPendingXfer.Count != 0);

	return;
}

void CTWainWrapper::CancelTransfer()
{
	TW_PENDINGXFERS twPend;
	CallTwainProc(&m_AppIdentity, &m_SourceIdentity, 
					DG_CONTROL, DAT_PENDINGXFERS, MSG_RESET,
					(TW_MEMREF)&twPend);
}

BOOL CTWainWrapper::EnableAutofeed()
{
	TW_CAPABILITY twCapability;
	if(!GetCapability(twCapability, CAP_FEEDERENABLED, TWON_ONEVALUE))
	{
		TW_STATUS status = GetStatus();
		if(TWCC_CAPUNSUPPORTED == status.ConditionCode)
		{
			m_bAutofeedEnable = FALSE;
			return FALSE;
		}

		m_bAutofeedEnable = FALSE;
		return FALSE;
	}
	
	pTW_ONEVALUE pValue = (pTW_ONEVALUE)GlobalLock(twCapability.hContainer);
	if(pValue->Item)
	{
		m_bAutofeedEnable = TRUE;
		GlobalUnlock(twCapability.hContainer);
		GlobalFree(twCapability.hContainer);
		return TRUE;
	}
	else
	{
		pValue->ItemType = TWTY_BOOL;
		pValue->Item = TRUE;
		GlobalUnlock(twCapability.hContainer);

		BOOL ret = SetCapability(twCapability);

		GlobalFree(twCapability.hContainer);

		if(ret)
		{
			m_bAutofeedEnable = TRUE;
			return TRUE;
		}
		else
		{
			m_bAutofeedEnable = FALSE;
			return FALSE;
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////
BOOL CTWainWrapper::IsValidDriver()
{
	return (m_hTwainDLL && m_lpDSM_Entry);
}

////////////////////////////////////////////////////
BOOL CTWainWrapper::CallTwainProc(pTW_IDENTITY pOrigin,pTW_IDENTITY pDest,
						   TW_UINT32 DG,TW_UINT16 DAT,TW_UINT16 MSG,
						   TW_MEMREF pData)
{
	if(IsValidDriver())
	{
		USHORT ret_val;
		ret_val = (*m_lpDSM_Entry)(pOrigin,pDest,DG,DAT,MSG,pData);
		m_returnCode = ret_val;
		if(ret_val != TWRC_SUCCESS)
		{
			(*m_lpDSM_Entry)(pOrigin,pDest,DG_CONTROL,DAT_STATUS,MSG_GET,&m_Status);
		}
		return (ret_val == TWRC_SUCCESS);
	}
	else
	{
		m_returnCode = TWRC_FAILURE;
		return FALSE;
	}
}
