// TWainHelper.cpp: implementation of the CTWainHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TWainApp.h"
#include "TWainHelper.h"

#include "CxImage/ximage.h"
#include "pdflib.h"

TABLEENTRY SetupXferType[MAX_SETUPXFERTYPE] =
{
	{"TWSX_NATIVE", TWSX_NATIVE},
	{"TWSX_FILE", TWSX_FILE},
	{"TWSX_MEMORY", TWSX_MEMORY}
};

TABLEENTRY PixelType[MAX_PIXELTYPE] =
{
	{"黑白模式", TWPT_BW},		//TWPT_BW
	{"灰度模式", TWPT_GRAY},	//TWPT_GRAY
	{"彩色模式", TWPT_RGB},		//TWPT_RGB
	{"TWPT_PALETTE", TWPT_PALETTE},
	{"TWPT_CMY", TWPT_CMY},
	{"TWPT_CMYK", TWPT_CMYK},
	{"TWPT_YUV", TWPT_YUV},
	{"TWPT_YUVK", TWPT_YUVK},
	{"TWPT_CIEXYZ", TWPT_CIEXYZ}
};

void CTWainHelper::LogMessage(char msg[])
{
#ifdef _DEBUG
	
	HFILE file = NULL;
	OFSTRUCT of;
	
	memset(&of, 0, sizeof(OFSTRUCT));
	
	if (OpenFile("c:\\twainapp.log", &of, OF_EXIST) == HFILE_ERROR)
	{
		//create if no exist
		file = OpenFile("c:\\twainapp.log", &of, OF_CREATE|OF_WRITE|OF_SHARE_DENY_NONE);
	}
	else
	{
		file = OpenFile("c:\\twainapp.log", &of, OF_WRITE|OF_SHARE_DENY_NONE);
	}
	
	//附加模式
	if (_llseek(file,0,FILE_END)==-1)
	{
		OutputDebugString("LogMessage _llseek failed\n");
	}
	
	if (_lwrite(file,msg,lstrlen(msg))==-1)
	{
		OutputDebugString("LogMessage _lwrite failed\n");
	}
	
	if (_lclose(file)==-1) 
	{
		OutputDebugString("LogMessage _lclose failed\n");
	}
	
#endif
	return;
}

BOOL CTWainHelper::SaveBMP(HGLOBAL _hDIB, char** picData, int* picLen)
{
	/*
	*	Declared locals
	*/
	
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER *pBIH = NULL;

	UINT i = 0, cbString = 0;

	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));

	/*
	*	Get a pointer to the beginning of the DIB
	*/
	pBIH = (BITMAPINFOHEADER*)GlobalLock(_hDIB);
	if(pBIH)
	{
		DWORD dwBytesWritten = 0l;

		/*
		* Fill in the Bitmap File Header
		*/
		bfh.bfType = ( (WORD) ('M' << 8) | 'B');
		
		/*
		*	Calculate the size of the bitmap including the palette
		*/
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
			((((pBIH->biWidth * pBIH->biBitCount + 31)/32) * 4) * pBIH->biHeight)
				+ pBIH->biClrUsed * sizeof(RGBQUAD);
		bfh.bfReserved1 = 0;
		bfh.bfReserved2 = 0;
		
		/*
		*	Offset to actual bits is after palette
		*/
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
			+ pBIH->biClrUsed * sizeof(RGBQUAD);

		// char** picData, int* picLen;
		*picLen = bfh.bfSize;
		*picData = new char[*picLen];

		memcpy(*picData, &bfh, sizeof(BITMAPFILEHEADER));
		memcpy(*picData + sizeof(BITMAPFILEHEADER), pBIH, bfh.bfSize - sizeof(BITMAPFILEHEADER));

		/*
		* Done now
		*/
		GlobalUnlock(_hDIB);
	}
	return TRUE;
}

void CTWainHelper::GetProportionSize(int nSrcWidth, int nSrcHeight, int nMaxWidth, int nMaxHeight, int* nNewWidth, int* nNewHeight)
{
	int w = nSrcWidth;
	int h = nSrcHeight;
	
	if(w > nMaxWidth)
	{
		*nNewWidth = nMaxWidth;
		*nNewHeight = h * nMaxWidth / w;
	}
	else
	{
		*nNewWidth = w;
		*nNewHeight = h;
	}
	
	w = *nNewWidth;
	h = *nNewHeight;
	
	if(h > nMaxHeight)
	{
		*nNewWidth = w * nMaxHeight / h;
		*nNewHeight = nMaxHeight;
	}
}

BOOL CTWainHelper::SaveAsPdf(const char* pFileName, std::vector< HANDLE > m_pic_handles)
{
	PDF *p;
    int image;
    const char *imagefile = "/pvf/pic.jpg";
	
    /* This is where font/image/PDF input files live. Adjust as necessary. */
    const char *searchpath = "./";
	
    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)!\n");
        return FALSE;
    }
	
    PDF_TRY(p){
		/* This means we must check return values of load_font() etc. */
		PDF_set_parameter(p, "errorpolicy", "return");
		
		/* This line is required to avoid problems on Japanese systems */
		PDF_set_parameter(p, "hypertextencoding", "host");

		if (PDF_begin_document(p, pFileName, 0, "") == -1) {
			printf("Error: %s\n", PDF_get_errmsg(p));
			return FALSE;
		}
		
		PDF_set_parameter(p, "SearchPath", searchpath);
		
		PDF_set_info(p, "Creator", "TWainApp");
		PDF_set_info(p, "Author", "Snail");
		PDF_set_info(p, "Title", "image sample (C)");

		std::vector< HANDLE >::iterator it;
		for(it = m_pic_handles.begin(); it != m_pic_handles.end(); it++)
		{
			//将Handle转换为CxImage对象
			HANDLE imageHandle = (HANDLE)(*it);
			CxImage cxImage;
			cxImage.CreateFromHANDLE(imageHandle);
			//cxImage.Save("C:\\bmp.bmp", CXIMAGE_FORMAT_BMP);
			cxImage.Save(".\\temp\\temp.jpg", CXIMAGE_FORMAT_JPG);

			char* imagedata = NULL;
			DWORD size = 0;

			CFile file(".\\temp\\temp.jpg", CFile::modeRead);
			size = file.GetLength();
			imagedata = new char[size];
			memset(imagedata, 0, size);
			file.Read(imagedata, size);
			file.Close();

			PDF_create_pvf(p, imagefile, 0, (const char*)imagedata, size, "");

			image = PDF_load_image(p, "auto", imagefile, 0, "");

			if (image == -1) {
				char logInfo[LOGSIZE];
				sprintf(logInfo, "Error: 加载PVF图像数据失败！错误号：%d, %s:%d\r\n", PDF_get_errmsg(p), __FILE__, __LINE__);
				CTWainHelper::LogMessage(logInfo);

				if(imagedata)
					delete imagedata;
				
				continue;
			}

			/* dummy page size, will be adjusted by PDF_fit_image() */
			PDF_begin_page_ext(p, 10, 10, "");
			PDF_fit_image(p, image, 0.0, 0.0, "adjustpage dpi {300 300}");
			PDF_end_page_ext(p, "");

			/* Delete the virtual file to free the allocated memory */
			PDF_delete_pvf(p, imagefile, 0);

			if(imagedata)
				delete imagedata;
		}

		PDF_end_document(p, "");    

		::DeleteFile(".\\temp\\temp.jpg");
	}	
    PDF_CATCH(p) {

		char logInfo[LOGSIZE];
		sprintf(logInfo, "EXCEPTION: PDFlib接口调用过程发生异常！错误信息：[%d] %s: %s, %s:%d\r\n", 
			PDF_get_errmsg(p), PDF_get_apiname(p), PDF_get_errmsg(p), __FILE__, __LINE__);
		CTWainHelper::LogMessage(logInfo);

        PDF_delete(p);
        return FALSE;
    }
	
    PDF_delete(p);
	return TRUE;
}

int CTWainHelper::Upload(const char* pParameter)
{
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    if( !CreateProcess( NULL, // No module name (use command line). 
			(char*)pParameter,		  // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi )             // Pointer to PROCESS_INFORMATION structure.
		) 
    {
		char logInfo[LOGSIZE];
		sprintf(logInfo, "EXCEPTION: 调用上传功能发生异常！, %s:%d\r\n", __FILE__, __LINE__);
        CTWainHelper::LogMessage(logInfo);

		return -1;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

	ULONG luExitCode = 0;
	GetExitCodeProcess(pi.hProcess, &luExitCode);

	char logInfo[LOGSIZE];
	sprintf(logInfo, "INFO: 调用上传功能完成，结果码：[%d][1?], %s:%d\r\n", luExitCode, __FILE__, __LINE__);
    CTWainHelper::LogMessage(logInfo);

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	return 0;
}