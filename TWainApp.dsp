# Microsoft Developer Studio Project File - Name="TWainApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TWainApp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TWainApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TWainApp.mak" CFG="TWainApp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TWainApp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TWainApp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TWainApp - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 pdflib.lib cximage.lib jasper.lib jbig.lib Jpeg.lib libdcr.lib mng.lib png.lib Tiff.lib zlib.lib gdiplus.lib cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TWainApp - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 pdflib.lib cximage.lib jasper.lib jbig.lib Jpeg.lib libdcr.lib mng.lib png.lib Tiff.lib zlib.lib gdiplus.lib cxcore.lib cv.lib ml.lib cvaux.lib highgui.lib cvcam.lib /nologo /subsystem:windows /debug /machine:I386 /out:"C:\Documents and Settings\HUJQ\×ÀÃæ\TWainApp\TWainApp_v2\Debug\TWainApp.exe" /pdbtype:sept /libpath:"./CxImageLibs/"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ENDIF 

# Begin Target

# Name "TWainApp - Win32 Release"
# Name "TWainApp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\OpenCVImageHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\PreViewFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TWainApp.cpp
# End Source File
# Begin Source File

SOURCE=.\TWainApp.rc
# End Source File
# Begin Source File

SOURCE=.\TWainAppDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TWainHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\TWainWrapper.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\OpenCV\cxcore\include\cxtypes.h
# End Source File
# Begin Source File

SOURCE=.\OpenCVImageHandle.h
# End Source File
# Begin Source File

SOURCE=.\pdflib.h
# End Source File
# Begin Source File

SOURCE=.\PreViewFrame.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RotateDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\twain.h
# End Source File
# Begin Source File

SOURCE=.\TWainApp.h
# End Source File
# Begin Source File

SOURCE=.\TWainAppDlg.h
# End Source File
# Begin Source File

SOURCE=.\TWainHelper.h
# End Source File
# Begin Source File

SOURCE=.\TWainWrapper.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\DELETE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\JP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LEFT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MOVEDOWN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MOVEUP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\QB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\QHB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RIGHT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TWainApp.ico
# End Source File
# Begin Source File

SOURCE=.\res\TWainApp.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
