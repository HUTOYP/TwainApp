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

	// !ͼƬȥ�ڱ�
	static void cv_RemoveBorder(string inPath, string outPath); 

	// !ͼƬ��ƫ
	static void cv_roteImgSelf(string inPath, string outPath, int& nRetValue);

	// !�ж��Ƿ�Ϊȫ��ҳ��
	static BOOL cv_isAllWhite(string inPath);

	// !ͼƬ˳ָ����ת��ʮ��
	static void cv_rotateImage(string inPath, string outPath);

	// !ͼƬ����
	static void cv_PictureCutting(string picPath, string outPath, int x, int y, int width, int hight);

	// !��ֵ��?
	static void cv_PictureBinary(string picPath, string picSavePath);

	// !FillWhite?
	static void cv_FillWhite(string inPath, string outPath, string templatewhitePath, int x, int y, int width, int hight);
	
private:
	// ȥ�ڱ߹��ܺ���
	static void removeblack(IplImage* img_src);
	// ��ƫ���ܺ���
	static int HoughLines2(IplImage * source,IplImage * src, string outPath);
	// ͼƬ��ת����
	static IplImage* rotateImage(IplImage* src, int angle, bool clockwise);
	// ��ֵ�����ܺ���
	static int BasicGlobalThreshold(int *pg, int start, int end);
	
	//���ĺ������㷨��
	static float FindMaxCountValue(std::vector<float> vec);

	static int Max(int a, int b, int c);
	static void trackbar(int pos);	
};

#endif // !defined(AFX_OPENCVIMAGEHANDLE_H__4B991AAF_FC25_4989_BCB4_D106E705FCFC__INCLUDED_)
