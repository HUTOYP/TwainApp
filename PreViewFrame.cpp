// PreViewFrame.cpp: implementation of the PreViewFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TWainApp.h"
#include "PreViewFrame.h"

#include "twain.h"

HWND hWnd;
BOOL bPreviewing = FALSE;

static	HGLOBAL		_ghDIB = NULL;		// handle to the current DIB
static	HBITMAP		hbm = NULL;			// holder handle to bit map passed from Source
static	HPALETTE	hDibPal = NULL;		// handle to palette
char	MainTitle[40];
BOOL	CapSupportedCaps = FALSE;
int		nPosition = 0;					/* Keep the scrollbar position */
BOOL	bSelection = TRUE;				/* To know the scrollbar in use TRUE for VSCROLL */
POINT	Point = {0,0};					/* Coordinate for the upper-left corner */
POINT	PtV = {0,0}, PtH = {0,0};
RECT	Rect, rc;
SCROLLINFO	sbInfoV, sbInfoH;
BOOL	bHideV = FALSE;
BOOL	bHideH = FALSE;

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: DibNumColors
//
// ARGS:    pv  pointer to bitmap data
//
// RETURNS: number of colors, 0, 2, 16, 256, in the DIB
//
// NOTES:
//
WORD DibNumColors (VOID FAR *pv)
{
	int Bits = 0;
	LPBITMAPINFOHEADER lpbi = NULL;
	LPBITMAPCOREHEADER lpbc = NULL;
	
	ASSERT(pv);
	
	lpbi = ((LPBITMAPINFOHEADER)pv);
	lpbc = ((LPBITMAPCOREHEADER)pv);
	
	//    With the BITMAPINFO format headers, the size of the palette
	//    is in biClrUsed, whereas in the BITMAPCORE - style headers, it
	//    is dependent on the bits per pixel ( = 2 raised to the power of
	//    bits/pixel).
	if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
	{
		if (lpbi->biClrUsed != 0)
		{
			return (WORD)lpbi->biClrUsed;
		}
		
		Bits = lpbi->biBitCount;
	}
	else
	{ 
		Bits = lpbc->bcBitCount;
	}
	
	switch (Bits)
	{
	case 1:
		return 2;
		
	case 4:
		return 16;
		
	case 8:
		return 256;
		
	default:
		// A 24 bitcount DIB has no color table
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CreateBIPalette
//
// ARGS:    lpbi        pointer to a BITMAPINFO structure
//
// RETURNS: hPalette    handle to the created palette
//                                        
// NOTES:   Given a Pointer to a BITMAPINFO struct will create a
//          GDI palette object from the color table.
//
HPALETTE CreateBIPalette (LPBITMAPINFOHEADER lpbi)
{          
	#define PALVERSION 0x0300
	#define MAXPALETTE 256

	HGLOBAL             hPal = NULL;
	LOGPALETTE          *pPal = NULL;
	HPALETTE            hPalette = NULL;
	WORD                nNumColors = 0;
	BYTE                Red = 0;
	BYTE                Green = 0;
	BYTE                Blue = 0;
	WORD                i = 0;
	RGBQUAD             FAR *pRgb = NULL;

	ASSERT(lpbi);

	if (!lpbi)
	{
		return NULL;
	}

	if (lpbi->biSize != sizeof(BITMAPINFOHEADER))
	{
		return NULL;
	}

	// Get a pointer to the color table and the number of colors in it 
	pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + (WORD)lpbi->biSize);
	nNumColors = DibNumColors(lpbi);

	if (nNumColors)
	{
		// Allocate for the logical palette structure
		//  SDH - 02/01/95 - For compatability with the Large model...
		//  pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE) + nNumColors *
		//                  sizeof(PALETTEENTRY));
		hPal = GlobalAlloc(GPTR,sizeof(LOGPALETTE) + nNumColors *
							sizeof(PALETTEENTRY));
		pPal = (LOGPALETTE*)(GlobalLock (hPal));
		if (!pPal)
		{
			return NULL;
		}

		pPal->palNumEntries = nNumColors;
		pPal->palVersion    = PALVERSION;

		// Fill in the palette entries from the DIB color table and
		// create a logical color palette.
		for (i = 0; i < nNumColors; i++)
		{
			pPal->palPalEntry[i].peRed   = pRgb[i].rgbRed;
			pPal->palPalEntry[i].peGreen = pRgb[i].rgbGreen;
			pPal->palPalEntry[i].peBlue  = pRgb[i].rgbBlue;
			pPal->palPalEntry[i].peFlags = (BYTE)0;
		}

		hPalette = CreatePalette(pPal);

		GlobalUnlock(hPal);
		GlobalFree(hPal);
	}
	else if (lpbi->biBitCount == 24)
	{
		// A 24 bitcount DIB has no color table entries so, set
		// the number of to the maximum value (256).
		nNumColors = MAXPALETTE;

		hPal = GlobalAlloc(GPTR,sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY));
		pPal = (LOGPALETTE*)GlobalLock (hPal);

		if (!pPal)
		{
			return NULL;
		}

		pPal->palNumEntries = nNumColors;
		pPal->palVersion    = PALVERSION;

		Red = Green = Blue = 0;

		// Generate 256 (= 8*8*4) RGB combinations to fill
		// the palette entries.
		for (i = 0; i < pPal->palNumEntries; i++)
		{
			pPal->palPalEntry[i].peRed   = Red;
			pPal->palPalEntry[i].peGreen = Green;
			pPal->palPalEntry[i].peBlue  = Blue;
			pPal->palPalEntry[i].peFlags = (BYTE)0;

			if (!(Red += 32))
			{
				if (!(Green += 32))
				{
					Blue += 64;
				}
			}
		}

		hPalette = CreatePalette(pPal);

		GlobalUnlock(hPal);
		GlobalFree(hPal);
	}
	return hPalette;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: FixUp -- Creates a device independent bitmap and frees the bmp
// handle memory. 
//
// ARGS:    hDib    handle to DIB
//
// RETURNS: hBitMap handle to a DDB
//
HBITMAP FixUp(HWND hWnd, HANDLE hDib)
{
	LPBITMAPINFOHEADER  lpDib = NULL, lpBi = NULL;
	LPSTR               lpBits = NULL;
	DWORD               dwColorTableSize = 0;
	HBITMAP             hBitMap = NULL;
	HDC                 hDC = NULL;
	
	ASSERT(hWnd);
	ASSERT(hDib);
	
	if (hDib == NULL)
	{
		return(NULL);
	}
	
	if ((lpDib = (LPBITMAPINFOHEADER) GlobalLock(hDib))==NULL)
	{
		return(NULL);
	}
	
	lpBi = lpDib;
	
	// Calculate the color table size, then point past the BITMAPINFOHEADER
	// and color table, to the byte array of bitmap bits.
	dwColorTableSize = (DWORD)(DibNumColors (lpDib) * sizeof(RGBQUAD));
	lpBits = (LPSTR)lpDib + lpBi->biSize + dwColorTableSize;
	
	// Create a logical palette based on the colors in the DIB header,
	// and realize it
	hDC = GetDC(hWnd);
	
	if (hDibPal = CreateBIPalette (lpBi))
	{
		SelectPalette (hDC, hDibPal, FALSE);
		RealizePalette (hDC);
	}
	
	//  Microsoft confirmed a defect in CreateDIBitmap whereas the function
	//  failed with handles to 1Meg or larger 1 color bitmaps.  The workaround
	//  is to create a device dependent bitmap whenever a 1 color bitmap
	//  is detected.  
	if (lpDib->biBitCount == 1)
	{
		// create a device dependent monochrome bitmap
		hBitMap = CreateBitmap((int)lpDib->biWidth,(int)lpDib->biHeight,1,1,lpBits);
		if(hBitMap)
		{
			SetDIBits(hDC,hBitMap,0,(UINT)lpDib->biHeight,lpBits,(LPBITMAPINFO)lpDib,DIB_RGB_COLORS);
		}
	}
	else
	{
		// Create a device-independent bitmap from the BMP
		hBitMap = CreateDIBitmap (hDC, lpDib, (LONG)CBM_INIT, (LPSTR)lpBits,
			(LPBITMAPINFO)lpDib, DIB_RGB_COLORS);
	}
	
	GlobalUnlock(hDib);
	
	ReleaseDC (hWnd, hDC);
	
	//	GlobalUnlock(hDib);
	
	// Return handle to device-dependent bitmap
	return(hBitMap);
}

/*
* Function: DrawScrollBar
* Author: Nancy L閠ourneau / J.F.L. Peripheral Solutions Inc. / TWAIN Working Group
* Date: May 29/1998
* Input: 
*		hWnd - handle to main app window
*		si - pointer to a SCROLLINFO structure
*		nScroll_Id - Type of scroll bar
*		bm - Current bitmap
* Output: none
* Comment:
*		This function is call in the WM_PAINT message and it draw the
*		scroll bar.
*/
void DrawScrollBar(HWND hWnd, SCROLLINFO* si, int nScroll_Id, BITMAP bm)
{
	BOOL Hide = FALSE;
	int AddSize = 0;
	
	ASSERT(hWnd);
	
	if (nScroll_Id == SB_VERT)
	{
		si->nMax = bm.bmHeight;
		
		AddSize = GetSystemMetrics(SM_CYMENU);
		AddSize += GetSystemMetrics(SM_CYCAPTION);
		AddSize += GetSystemMetrics(SM_CXFRAME);
		AddSize += 1;
		if(bHideV == FALSE)
		{
			AddSize += GetSystemMetrics(SM_CYHSCROLL);
		}
		
		si->nPage = (Rect.bottom - Rect.top) - 
			(bm.bmHeight / Rect.bottom + AddSize);
	}
	else
	{
		si->nMax = bm.bmWidth;
		
		AddSize = GetSystemMetrics(SM_CYFRAME); 
		AddSize += 1;
		if (bHideH == FALSE)
		{
			AddSize += GetSystemMetrics(SM_CXVSCROLL); 				
		}
		
		si->nPage = (Rect.right - Rect.left) - 
			(bm.bmWidth / Rect.right + AddSize);
	}
	
	si->cbSize = sizeof(SCROLLINFO);
	si->fMask = SIF_ALL;
	SetScrollInfo(hWnd, nScroll_Id, si, TRUE);
	
	if (nScroll_Id == SB_VERT)
	{
		if((Rect.bottom - Rect.top) > bm.bmHeight)
		{
			Hide = TRUE;
		}
	}
	else
	{
		if((Rect.right - Rect.left) > bm.bmWidth)
		{
			Hide = TRUE;
		}
	}
	
	if(Hide == FALSE)
	{
		SetScrollInfo(hWnd, nScroll_Id, si, TRUE);
		ShowScrollBar(hWnd, nScroll_Id, TRUE);
	}
	else
	{
		ShowScrollBar(hWnd, nScroll_Id, FALSE);
	}
	return;
}

/*
* Function:	OnScrollBar
* Author:	Nancy L閠ourneau / J.F.L. Peripheral Solutions Inc. / TWAIN Working Group
* Date:		March 1998
* Input:	hWnd - Handle to window with scroll bar.
*			nScroll_Id - Type of scroll bar.
*			code - Scroll bar value.
*			Pos	- Scroll box position.
* Output:	
* Comments: Intercept the scrollbar code, depend what the user press.
*/
void OnScrollBar(HWND hWnd, int nScroll_Id, UINT code,
				 int Pos, SCROLLINFO *sbInfo)
{
	int  nTmp = 0;
	int nPageSize = 0;

	ASSERT(hWnd);

	/*
	* To keep the coordinate  of the upper-left corner.
	* Put the value of the coordinate in a tempory variable
	*/
	if (nScroll_Id == SB_VERT)
	{
		nTmp = Point.y;
	}
	else
	{
		nTmp = Point.x;
	}

	#ifdef _WIN32
	{
		int AddSize = 0;

		GetScrollInfo(hWnd, nScroll_Id, sbInfo);
		nPageSize = sbInfo->nPage;

		switch(code)
		{
			/* 
			* When you press the "HOME" key.
			*/
			case SB_TOP:
				sbInfo->nPos = sbInfo->nMin;
				nTmp = sbInfo->nMin;
				break;

			/*
			* When you press the "END" key.
			*/
			case SB_BOTTOM:
				sbInfo->nPos = sbInfo->nMax;
				nTmp = sbInfo->nMax;
				break;						 

			case SB_LINEUP:
				if ((int)sbInfo->nPos > 0)
				{
					sbInfo->nPos -= 1;
					nTmp -= 1;
				}
				break;

			case SB_LINEDOWN:
				if ((int)sbInfo->nPos < sbInfo->nMax)
				{
					sbInfo->nPos += 1;
					nTmp += 1;
				}
				break;

			case SB_PAGEUP:
				if ((UINT)sbInfo->nPos < sbInfo->nPage)
				{
					nPageSize = sbInfo->nPos;
				}
				sbInfo->nPos -= nPageSize;
				nTmp -= nPageSize;
				break;

			case SB_PAGEDOWN:
				if ((UINT)sbInfo->nPos > sbInfo->nMax - sbInfo->nPage)
				{
					nPageSize = sbInfo->nMax - sbInfo->nPage;
				}
				sbInfo->nPos += nPageSize;
				nTmp += nPageSize;
				break;

			case SB_THUMBTRACK:
				sbInfo->nPos = sbInfo->nTrackPos;
				nTmp = sbInfo->nTrackPos;
				break;

			case SB_ENDSCROLL:
				return; 
		}

		/*
		* Put back the value in the good coordinate.
		*/
		if (nScroll_Id == SB_VERT)
		{
			if(code == SB_PAGEUP || code == SB_PAGEDOWN)
			{
				AddSize = GetSystemMetrics(SM_CYMENU);
				AddSize += GetSystemMetrics(SM_CYCAPTION);
				AddSize += GetSystemMetrics(SM_CXFRAME);
				if(bHideV == FALSE)
				{
					AddSize += GetSystemMetrics(SM_CYHSCROLL);
				}
				if(sbInfo->nPos > sbInfo->nMax - Rect.bottom)
				{
					Point.y = sbInfo->nMax - Rect.bottom + AddSize;
				}
				else
				{
					Point.y = nTmp;
				}
			}
			else
			{
				Point.y = nTmp;
			}
		}
		else
		{						
			if(code == SB_PAGEUP || code == SB_PAGEDOWN)
			{
				AddSize = GetSystemMetrics(SM_CYFRAME); 
				if (bHideH == FALSE)
				{
					AddSize += GetSystemMetrics(SM_CXVSCROLL); 				
				}
				if(sbInfo->nPos > sbInfo->nMax - Rect.right)
				{
					Point.x = sbInfo->nMax - Rect.right + AddSize;
				}
				else
				{
					Point.x = nTmp;
				}
			}
			else
			{
				Point.x = nTmp;
			}
		}
		SetScrollInfo(hWnd, nScroll_Id, sbInfo, TRUE);
	}

	#else //_WIN32
	{
		int nTemp = 0;
		int nScrPos = 0;
		int nDiff = 0;
		int nMax = 0;
		intnMin = 0;

		nPosition = GetScrollPos(hWnd, nScroll_Id);
		GetScrollRange(hWnd, nScroll_Id, &nMin, &nMax);
		switch(code)
		{
			/* 
			* When you press the "HOME" key.
			*/
			case SB_TOP:
				nPosition = nMin;
				Point.y = nMin;
				Point.x = nMin;
				nScrPos = nMax;
				nTmp = nMin;
				break;

			/*
			* When you press the "END" key.
			*/
			case SB_BOTTOM:
				nPosition = nMax;
				Point.y = nMax;
				Point.x = nMax;
				nScrPos = nMax;
				nTmp = nMax;
				break;						 

			case SB_LINEUP:
				if ((int)nPosition > 0)
				{
					nPosition -= 1;
					nTmp -= 1;
					nScrPos = 1;
					nDiff -= 1;		 
				}
				break;

			case SB_LINEDOWN:
				if ((int)nPosition < nMax)
				{
					nPosition += 1;
					nTmp += 1;
					nScrPos = -1;
					nDiff += 1;
				}
				break;

			case SB_PAGEUP:
				if ((int)nPosition > 0)
				{
					nPosition -= nPageSize;
					nTmp -= nPageSize;
					nScrPos = nPageSize;		 
					nDiff -= nPageSize;
				}
				break;

			case SB_PAGEDOWN:
				if ((int)nPosition < nMax)
				{
					nPosition += nPageSize;
					nTmp += nPageSize;
					nScrPos = -nPageSize;
					nDiff += nPageSize;
				}
				break;

			case SB_THUMBTRACK:
				nTemp = nPosition;
				nPosition = Pos;
				nScrPos = (nTemp - nPosition);
				nDiff = -(nTemp - nPosition);
				break;

			case SB_THUMBPOSITION:
				nPosition = Pos;
				nTmp = Pos;
				break;
		}

		/*
		* Put back the value in the good coordinate.
		*/
		if (nScroll_Id == SB_VERT)
		{
			Point.y = nTmp;
		}
		else
		{						
			Point.x = nTmp;
		}
		SetScrollPos(hWnd, nScroll_Id, nPosition, TRUE);

		/*
		* Condition for the vertical scroll
		*/
		if (nScroll_Id == SB_VERT)
		{
			if (nDiff > 0)
			{
				rc.left = Rect.left;
				rc.top = Rect.bottom - nDiff;
				rc.right = Rect.right;
				rc.bottom = Rect.bottom;
			}
			else
			{
				rc.left = Rect.left;
				rc.top = Rect.top;
				rc.right = Rect.right;
				rc.bottom = Rect.top + nDiff;
			}
			ScrollWindow(hWnd, 0, nScrPos, NULL, NULL);
		}
		/*
		* Condition for the horizontal scroll
		*/
		else
		{
			if (nDiff > 0)
			{
				rc.left = Rect.right - nDiff;
				rc.top = Rect.top;
				rc.right = Rect.right;
				rc.bottom = Rect.bottom;
			}
			else
			{
				rc.left = Rect.left;
				rc.top = Rect.top;
				rc.right = Rect.left + nDiff;
				rc.bottom = Rect.bottom;
			}
			ScrollWindow(hWnd, nScrPos, 0, NULL, NULL);		
		}
	}
	#endif //_WIN32 
	return;
}

LRESULT FAR PASCAL TW_MainWndProc (HWND hWnd,
                                   UINT iMessage,
                                   WPARAM wParam,
                                   LPARAM lParam)
{
	PAINTSTRUCT     ps;
	HDC             hDC = NULL, hMemDC = NULL;
	BITMAP          bm;
	FARPROC         lpProcAbout = NULL;        // pointer to the "About" function
	FARPROC         lpProcAuto = NULL;
	int             ret = 0;
	static int      FirstDraw = 0;          // flag used to control window redraw
	TW_UINT16 wmId = 0;
	TW_UINT16 wmEvent = 0;
	#ifdef WIN32
		wmId = LOWORD(wParam);
		wmEvent = HIWORD (wParam);
	#else
		wmId = wParam;
		wmEvent = HIWORD (lParam);
	#endif

	memset(&ps, 0, sizeof(PAINTSTRUCT));
	memset(&bm, 0, sizeof(BITMAP));

	ASSERT(hWnd);

	switch (iMessage)
	{
//-----------------------------------------------------------------
 		case WM_CLOSE:
 		case WM_ENDSESSION:
			// Always try to take SM and Source down with you
 			
			/*
			*	Free any previous DIB image
			*	应该释放，但不应该在这里
			*/
// 			if(_ghDIB)
// 			{
// 				GlobalFree(_ghDIB);
// 				_ghDIB = NULL;
// 			}
// 
// 			if (hbm)
// 			{
// 				DeleteObject(hbm);
// 			}
// 			hbm = NULL;
// 
// 			if (hDibPal)
// 			{
// 				DeleteObject(hDibPal);
// 			}
// 			hDibPal = NULL;

			/////////////////////////////

			bPreviewing = FALSE;
 			DestroyWindow(hWnd);

			break;
        
		case WM_DESTROY:	
			break;        
//-----------------------------------------------------------------
		case WM_PALETTECHANGED:
			InvalidateRect (hWnd, NULL, FALSE);
			break;

		//case WM_SYSCOLORCHANGE:
		//	Ctl3dColorChange();
		//	break;
//-----------------------------------------------------------------
		case USER_XFERDONE:            
			// get private message, hbm in wParam, from DCA_ACQ.C convert from
			// DCA_ACQ.C returns a DIB, so change to DDB for sample App
			if (wParam != NULL)
			{
				_ghDIB = (HGLOBAL)wParam;
				hbm = FixUp (hWnd, (HANDLE)wParam);

				// Test for valid handle
				if (hbm == NULL)
				{
					break;
				}

				FirstDraw = TRUE;

				InvalidateRect(hWnd,NULL,TRUE);
			}
			break;
//-----------------------------------------------------------------
		case WM_PAINT:
			{
				#ifdef _WIN32
					int AddHeight = 0, AddWidth = 0;
				#endif

				GetWindowRect(hWnd, &Rect);

				hDC = BeginPaint(hWnd, &ps);

				if (hbm)
				{
					hMemDC = CreateCompatibleDC(hDC);

					/*
					* setup correct palette
					*/
					if (hDibPal)
					{
						SelectPalette (hDC, hDibPal, FALSE);
						SelectPalette (hMemDC, hDibPal, FALSE);
						RealizePalette (hDC);
					}   
             
					GetObject(hbm, sizeof(BITMAP), (LPSTR) &bm);

					if (FirstDraw==TRUE)
					{
						SetWindowPos (hWnd, (HWND)NULL, Rect.left, Rect.top,
												Rect.right,Rect.bottom,
												SWP_NOMOVE | SWP_NOZORDER);

						// flag to adjust size only when first drawn
						ValidateRect (hWnd, NULL);
						FirstDraw = FALSE;
					}

					SelectObject(hMemDC, hbm);

					#ifdef _WIN32
						/*
						* True is the vertical scroll bar action
						*/
						if (bSelection)
						{
							BitBlt(hDC, -Point.x, -sbInfoV.nPos, bm.bmWidth, 
										bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
						}
						/*
						* False is the horizontal scroll bar action
						*/
						else
						{
							BitBlt(hDC, -sbInfoH.nPos, -Point.y, bm.bmWidth, 
										bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
						}

						DrawScrollBar(hWnd, &sbInfoV, SB_VERT, bm);
						DrawScrollBar(hWnd, &sbInfoH, SB_HORZ, bm);

					#else //_WIN32

						/*
						* True is the vertical scroll bar action
						*/
						if (bSelection)
						{
							BitBlt(hDC, -Point.x, -nPosition, bm.bmWidth, 
										bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
						}
						/*
						* False is the horizontal scroll bar action
						*/
						else
						{
							BitBlt(hDC, -nPosition, -Point.y, bm.bmWidth, 
										bm.bmHeight, hMemDC, 0, 0, SRCCOPY);
						}
						SetScrollRange(hWnd, SB_VERT, 0, bm.bmHeight, FALSE);
						SetScrollRange(hWnd, SB_HORZ, 0, bm.bmWidth, FALSE);

					#endif //_WIN32

					DeleteDC(hMemDC);

				}
				else
				{
					ShowScrollBar(hWnd, SB_VERT, FALSE);
					ShowScrollBar(hWnd, SB_HORZ, FALSE);
				}
				EndPaint(hWnd, &ps);	
			}	 
			break;
//-----------------------------------------------------------------
		case WM_SIZING:
			{
				LPRECT lprc = (LPRECT)lParam;
				LONG Width = 0;
				LONG Height = 0;
				int AddHeight = 0, AddWidth = 0;

				if (hbm)
				{
					GetWindowRect(hWnd, &Rect);
					GetObject(hbm, sizeof(BITMAP), (LPSTR)&bm);
				}
				else
				{
					bm.bmHeight = 0;
					bm.bmWidth = 0;
				}

				AddWidth = GetSystemMetrics(SM_CXVSCROLL);
				AddWidth += (2 * (GetSystemMetrics(SM_CYFRAME)));
				AddHeight = GetSystemMetrics(SM_CYMENU);
				AddHeight += GetSystemMetrics(SM_CYHSCROLL);
				AddHeight += GetSystemMetrics(SM_CYCAPTION);
				AddHeight += (2 * (GetSystemMetrics(SM_CXFRAME)));

				Height = Rect.top + bm.bmHeight;
				Width = Rect.left + bm.bmWidth;

				/*
				* Vertical scroll bar
				*/
				if (bm.bmWidth > 400)
				{
					if (lprc->right > Width)
					{
						if (lprc->bottom > Height)
						{
							AddWidth -= GetSystemMetrics(SM_CXVSCROLL);
							AddHeight -= GetSystemMetrics(SM_CYHSCROLL);							
						}
						bHideV = TRUE;
						lprc->right = Width + AddWidth;
					}
					else if(lprc->right < Rect.left + 400)
					{
						bHideV = TRUE;
						lprc->right = Rect.left + 400;
					}
					else
					{
						bHideV = FALSE;
					}
				}
				else
				{
					bHideV = TRUE;
					lprc->right = Rect.left + 400;
				}

				if(bHideV == FALSE)
				{
					SetScrollInfo(hWnd, SB_VERT, &sbInfoV, TRUE);
					ShowScrollBar(hWnd, SB_VERT, TRUE);
				}
				else
				{
					ShowScrollBar(hWnd, SB_VERT, FALSE);
				}

				/*
				* Horizontal scroll bar
				*/

				if(bm.bmHeight > 300)
				{
					if(lprc->bottom > Height)
					{
						bHideH = TRUE;
						lprc->bottom = Height + AddHeight;
					}
					else if(lprc->bottom < Rect.top + 300)
					{
						bHideH = TRUE;
						lprc->bottom = Rect.top + 300;
					}
					else
					{
						bHideH = FALSE;
					}
				}					
				else
				{
					bHideH = TRUE;
					lprc->bottom = Rect.top + 300;
				}

				if(bHideH == FALSE)
				{
					SetScrollInfo(hWnd, SB_HORZ, &sbInfoH, TRUE);
					ShowScrollBar(hWnd, SB_HORZ, TRUE);
				}
				else
				{
					ShowScrollBar(hWnd, SB_HORZ, FALSE);
				}
			}
			break;
//-----------------------------------------------------------------
		/*
		* Generate the vertical scroll bar codes.
		*/
		case WM_VSCROLL:
			bSelection = TRUE;
			OnScrollBar(hWnd, SB_VERT, LOWORD(wParam), 
								HIWORD(wParam), &sbInfoV);
			#ifdef _WIN32
				InvalidateRect(hWnd, FALSE, FALSE);
			#else //_WIN32
				InvalidateRect(hWnd, &rc, FALSE);
			#endif //_WIN32
			break;
//-----------------------------------------------------------------
		/*
		* Generate the Horizontal scroll bar codes.
		*/
		case WM_HSCROLL:
			bSelection = FALSE;
			OnScrollBar(hWnd, SB_HORZ, LOWORD(wParam),
								HIWORD(wParam), &sbInfoH);
			#ifdef _WIN32
				InvalidateRect(hWnd, FALSE, FALSE);
			#else //_WIN32
				InvalidateRect(hWnd, &rc, FALSE);
			#endif //_WIN32
			break;
//---------------------------------------------------------------- 
		default:
			return DefWindowProc (hWnd, iMessage, wParam, lParam);
	}   

	return 0L ;
}

BOOL CreatePreViewFrame(HINSTANCE instance)
{
	WNDCLASS  wc;
	memset(&wc, 0, sizeof(WNDCLASS));

	// Set up application's main window
	wc.style         = NULL;								// no style bits
	wc.lpfnWndProc   = (WNDPROC)TW_MainWndProc;				// name of window proc
	wc.cbClsExtra    = 0;									// no extra bits
	wc.cbWndExtra    = 0;
	wc.hInstance     = instance;							// handle to cur instance
	wc.hIcon         = LoadIcon( NULL, "TW_APP_ICO");		// load resources
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);				// load mouse icon
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);     // use white backgnd
	wc.lpszMenuName  = NULL;									// no menu
	wc.lpszClassName = "TW_App_Preview";						// class named

	if(!RegisterClass(&wc))
	{
		//return FALSE;
	}

	hWnd = CreateWindow("TW_App_Preview", "扫描预览", WS_VSCROLL | 
						WS_HSCROLL | WS_OVERLAPPEDWINDOW,
						0, 0, 600, 380, NULL, NULL, instance, NULL);

	bPreviewing = TRUE;

	ShowWindow (hWnd, TRUE);
	UpdateWindow (hWnd);

	return TRUE;
}