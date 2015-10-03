// OpenCVImageHandle.h: interface for the OpenCVImageHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENCVIMAGEHANDLE_H__4B991AAF_FC25_4989_BCB4_D106E705FCFC__INCLUDED_)
#define AFX_OPENCVIMAGEHANDLE_H__4B991AAF_FC25_4989_BCB4_D106E705FCFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "highgui.h"
#include "cv.h"

#include <vector>
#include <algorithm>
using namespace std;

class OpenCVImageHandle  
{
public:
	OpenCVImageHandle();
	virtual ~OpenCVImageHandle();

	// !图片去黑边
	static void cv_RemoveBorder(string inPath, string outPath); 

	// !图片纠偏
	static void cv_roteImgSelf(string inPath, string outPath, int& nRetValue);

	// !判断是否为全白页面
	static BOOL cv_isAllWhite(string inPath);

	// !图片顺指针旋转九十度
	static void cv_rotateImage(string inPath, string outPath);

	// !图片剪切
	static void cv_PictureCutting(string picPath, string outPath, int x, int y, int width, int hight);

	// !二值化?
	static void cv_PictureBinary(string picPath, string picSavePath);

	// !FillWhite?
	static void cv_FillWhite(string inPath, string outPath, string templatewhitePath, int x, int y, int width, int hight);
	
private:
	// 去黑边功能函数
	static void removeblack(IplImage* img_src);
	// 纠偏功能函数
	static int HoughLines2(IplImage * source,IplImage * src, string outPath);
	// 图片旋转函数
	static IplImage* rotateImage(IplImage* src, int angle, bool clockwise);
	// 二值化功能函数
	static int BasicGlobalThreshold(int *pg, int start, int end);
	
	//核心函数（算法）
	static float FindMaxCountValue(std::vector<float> vec);

	static int Max(int a, int b, int c);
	static void trackbar(int pos);	
};

#endif // !defined(AFX_OPENCVIMAGEHANDLE_H__4B991AAF_FC25_4989_BCB4_D106E705FCFC__INCLUDED_)
