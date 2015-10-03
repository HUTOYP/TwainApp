; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTWainAppDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TWainApp.h"

ClassCount=5
Class1=CTWainAppApp
Class2=CTWainAppDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_TWAINAPP_DIALOG
Resource2=IDR_MAINFRAME
Class4=CDialogDetail
Resource3=IDD_ABOUTBOX
Class5=CRotateDialog
Resource4=IDD_DIALOG_XZ

[CLS:CTWainAppApp]
Type=0
HeaderFile=TWainApp.h
ImplementationFile=TWainApp.cpp
Filter=N

[CLS:CTWainAppDlg]
Type=0
HeaderFile=TWainAppDlg.h
ImplementationFile=TWainAppDlg.cpp
Filter=D
LastObject=IDC_LIST_IMAGE
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=TWainAppDlg.h
ImplementationFile=TWainAppDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TWAINAPP_DIALOG]
Type=1
Class=CTWainAppDlg
ControlCount=33
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_SOURCE,edit,1350631552
Control3=IDC_BUTTON_SELECT,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_CHECK_SHOW,button,1342242819
Control6=IDC_STATIC,static,1342308352
Control7=IDC_COMBO_PAGE,combobox,1344340227
Control8=IDC_STATIC,static,1073872896
Control9=IDC_EDIT_ROTATION,edit,1082196096
Control10=IDC_SPIN_ROTATION,msctls_updown32,1073741872
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO_COLOR,combobox,1344340227
Control13=IDC_STATIC,static,1342308352
Control14=IDC_BUTTON_SCAN,button,1342242816
Control15=IDC_BUTTON_INSCAN,button,1342242816
Control16=IDC_BUTTON_CANCLE,button,1073807360
Control17=IDC_BUTTON_SAVE,button,1342242816
Control18=IDC_STATIC,button,1342177287
Control19=IDC_EDIT_RESOLUTION,edit,1350639744
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC_IMAGE,static,1350567438
Control22=IDC_LIST_IMAGE,SysListView32,1350633740
Control23=IDC_BUTTON_LEFT90,button,1342242944
Control24=IDC_BUTTON_RIGHT90,button,1342242944
Control25=IDC_BUTTON_MOVEUP,button,1342246784
Control26=IDC_BUTTON_MOVEDOWN,button,1342242944
Control27=IDC_BUTTON_DELETE,button,1342242944
Control28=IDC_STATIC,static,1342177287
Control29=IDC_BUTTON_QHB,button,1342242944
Control30=IDC_BUTTON_JP,button,1342242944
Control31=IDC_STATIC,static,1342177287
Control32=IDC_BUTTON_QB,button,1342242944
Control33=IDC_BUTTON_CLOSE,button,1342242816

[CLS:CDialogDetail]
Type=0
HeaderFile=DialogDetail.h
ImplementationFile=DialogDetail.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_STATIC_IMAGE
VirtualFilter=dWC

[DLG:IDD_DIALOG_XZ]
Type=1
Class=CRotateDialog
ControlCount=8
Control1=IDC_SLIDER,msctls_trackbar32,1342177281
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308354
Control6=IDC_STATIC,static,1342308353
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC_VALUE,static,1342308352

[CLS:CRotateDialog]
Type=0
HeaderFile=RotateDialog.h
ImplementationFile=RotateDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CRotateDialog
VirtualFilter=dWC

