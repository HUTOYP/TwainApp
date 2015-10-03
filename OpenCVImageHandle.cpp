// OpenCVImageHandle.cpp: implementation of the OpenCVImageHandle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TWainApp.h"
#include "OpenCVImageHandle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OpenCVImageHandle::OpenCVImageHandle()
{

}

OpenCVImageHandle::~OpenCVImageHandle()
{

}

int OpenCVImageHandle::Max(int a, int b, int c)
{
	int temp = a;
	temp = a > b ? a : b; 
	return temp = temp > c ? temp : c; 
}

float OpenCVImageHandle::FindMaxCountValue(std::vector<float> vec)
{
	float fVal = 0.000000; 
	int iCount = 0;

	vector<float>::iterator its = vec.begin();
	vector<float>::iterator its2;
	for (; its != vec.end(); ++its)
	{
		float ftempVal;
		float ftempTotal = *its;
		int iThisCount = 1;
		for (its2 = vec.begin(); its2 != vec.end(); its2++)
		{
			float ftemp = abs(*its - *its2);
			if ((*its) == (*its2) || ftemp < 0.500000)
			{
				iThisCount++;
				ftempTotal += *its2;
				ftempVal = (float)ftempTotal/iThisCount;
			}
		}
		if (iThisCount>vec.size()*2/5)  //����������֮һ����ֱ�ӷ��ظýǶ�ֵ
		{
			return ftempVal;
		}
		else
		{
			if (iCount<iThisCount)
			{		
				iCount = iThisCount;
				fVal = ftempVal;
			}
			else if (iCount == iThisCount)
			{
				float ftempend = abs(fVal-ftempVal);
				if (ftempend<0.500000)
				{
					iCount = iThisCount;
					fVal = (ftempVal+fVal)/2;
				}
			}
		}
	}
	return fVal;
}

//��ֵ��
int OpenCVImageHandle::BasicGlobalThreshold(int *pg, int start, int end)
{                                           
	int  i,t,t1,t2,k1,k2;
	double u,u1,u2;

	t=0;     u=0;
	for (i=start;i<end;i++) {
		t+=pg[i];		
		u+=i*pg[i];
	}
	k2=(int) (u/t);                          

	do {
		k1=k2;
		t1=0;     u1=0;
		for (i=start;i<=k1;i++) {             
			t1+=pg[i];	
			u1+=i*pg[i];
		}
		t2=t-t1;
		u2=u-u1;
		if (t1) u1=u1/t1;                     
		else u1=0;
		if (t2) u2=u2/t2;                     
		else u2=0;
		k2=(int) ((u1+u2)/2);                 
	}
	while(k1!=k2);                           

	return(k1);                              
}

void OpenCVImageHandle::removeblack(IplImage* img_src)
{
	//�����׵�ַ������洢�ռ�
	//IplImage* cvCreateImage( CvSize size, int depth, int channels );
	IplImage* gray = cvCreateImage(cvGetSize(img_src), 8, 1);
	//Opencv�����ɫ�ռ�ת������������ʵ��RGB��ɫ��HSV,HSI����ɫ�ռ��ת����Ҳ����ת��Ϊ�Ҷ�ͼ��
	//void cvCvtColor( const CvArr* src, CvArr* dst, int code );
	//src :�����8-bit, 16-bit�� 32-bit�������ȸ�����Ӱ��
	//dst :�����8-bit, 16-bit�� 32-bit�������ȸ�����Ӱ��
	//code :ɫ�ʿռ�ת����ģʽ����code��ʵ�ֲ�ͬ���͵���ɫ�ռ�ת����
	//����CV_BGR2GRAY��ʾת��Ϊ�Ҷ�ͼ��CV_BGR2HSV��ͼƬ��RGB�ռ�ת��ΪHSV�ռ䡣
	//���е�codeѡ��CV_BGR2GRAYʱ��dst��Ҫ�ǵ�ͨ��ͼƬ����codeѡ��CV_BGR2HSVʱ������8λͼ����Ҫ��RGBֵ��һ����0-1֮�䡣
	//�����õ�HSVͼ�е�H��Χ����0-360��S��V�ķ�Χ��0-1��
	cvCvtColor( img_src, gray, CV_BGR2GRAY);

	/*
	���� cvThreshold �Ե�ͨ������Ӧ�ù̶���ֵ������
	�ú����ĵ���Ӧ���ǶԻҶ�ͼ�������ֵ�����õ���ֵͼ��
	(cvCmpS Ҳ���Դﵽ��Ŀ��) ������ȥ��������������˺�С��ܴ�����ֵ��ͼ��㡣
	������֧�ֵĶ�ͼ��ȡ��ֵ�ķ����� threshold_type ȷ����

	void cvThreshold( const CvArr* src, CvArr* dst, double threshold, double max_value, int threshold_type );
	src��ԭʼ���� (��ͨ�� , 8-bit of 32-bit ������)��
	dst��������飬������ src ������һ�£�����Ϊ 8-bit��
	threshold����ֵ
	max_value��ʹ�� CV_THRESH_BINARY �� CV_THRESH_BINARY_INV �����ֵ��
	threshold_type����ֵ���� threshold_type=CV_THRESH_BINARY:
	��� src(x,y)>threshold ,dst(x,y) = max_value; ����,dst(x,y)=0;
	threshold_type=CV_THRESH_BINARY_INV:
	��� src(x,y)>threshold, dst(x,y) = 0; ����,dst(x,y) = max_value.
	threshold_type=CV_THRESH_TRUNC:
	��� src(x,y)>threshold��dst(x,y) = threshold; ����dst(x,y) = src(x,y).
	threshold_type=CV_THRESH_TOZERO:
	���src(x,y)>threshold��dst(x,y) = src(x,y) ; ���� dst(x,y) = 0��
	threshold_type=CV_THRESH_TOZERO_INV:
	*/
	cvThreshold(gray, gray, 110, 255, CV_THRESH_BINARY);
	//cvAdaptiveThreshold ?

/*	int pg[256];
	memset(pg, 0, 256*sizeof(int));
	for (int i=0; i<gray->imageSize; i++)      
		pg[(unsigned char)gray->imageData[i]]++;
	int thre = BasicGlobalThreshold(pg, 0, 256);    
	cvThreshold(gray, gray, thre, 255, CV_THRESH_BINARY); 
*/
	//���� cvSmooth ��ʹ�ü�ģ�����������ű任��ģ������ֵģ������˹ģ����˫���˲����κ�һ�ַ���ƽ��ͼ��
	//void cvSmooth( const CvArr* src, CvArr* dst,int smoothtype=CV_GAUSSIAN,int param1=3, int param2=0, double param3=0 ,double param4=0);
	cvSmooth(gray,gray,CV_MEDIAN,3,0,0,0);

	//���������Ǳ��ز������������⿪�ٴ洢�ռ����˼����ʴ�����ظ����� (iterations) ��. �Բ�ɫͼ��ÿ����ɫͨ����������
	//void cvErode( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 );
	//src������ͼ��.
	//dst�����ͼ��.
	//element�����ڸ�ʴ�ĽṹԪ�ء���Ϊ NULL, ��ʹ�� 3��3 �����εĽṹԪ��
	//iterations����ʴ�Ĵ���
	cvErode(gray,gray,0,2); 
	/*
	void cvDilate( const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 );
	src:����ͼ��.
	dst:���ͼ��.
	element:�ṹԪ�ء���Ϊ NULL, ��ʹ��Ĭ�ϵ�3��3 �����Σ�ê�����м�ĽṹԪ�أ�������������
	iterations:���͵Ĵ���
	���� cvDilate ������ͼ��ʹ��ָ���ĽṹԪ�������ͣ��ýṹ����ÿ���������ֵ���ص��������״��
	˵����
	ʹ������ṹԪ������ͼ�񣬺����ڵ����п���������ͼ����ֱ�ӽ��в�������������·�ʽ���ã�cvDilate (img1, img1)��
	���Ϳ����ظ����� (iterations) ��. �Բ�ɫͼ��ÿ����ɫͨ����������
	*/
	cvDilate(gray,gray,0,1);

	int i;

	int width  = cvGetSize(img_src).width;
	int height = cvGetSize(img_src).height;
	
	//���ú��ʵ���ֵ������Ҫ
	//��
	for (i = 0; i <= 5; i++)
	{
		for (int j = 0; j< width ; j++)
		{
			int channel0 = (uchar)(gray->imageData + i*gray->widthStep)[j*gray->nChannels + 0];
			if (channel0 == 0)
			{
				int channel0 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 0];
				int channel1 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 1];
				int channel2 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 2];

				if (channel0 < 100 && channel1 < 100 && channel2 < 100)
				{
					/*
					void cvFloodFill(
					IplImage* src,
					CvPoint seedPoint, //��ˮ���ӵ�seedPoint��ʼʵ���㷨
					CvScalar newVal, //���ص㱻Ⱦɫ��ֵ
					CvScalar loDiff = cvScalarAll(0), //�±��--��Ⱦɫ�����ڵ��ȥloDiff
					CvScalar upDiff = cvScalarAll(0), //�ϱ��--��Ⱦɫ�����ڵ����upDiff
					CvConnectedComp* comp = NULL, //���comp����NULL����ô��CvConnectedComp������Ϊ����������ͳ������
					int flags = 4,
					CvArr* mask = NULL
					);
					*/
					//cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(5,5,5,0),NULL,4);
					cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(250,250,250,0),NULL, 4);
				}
			}
		}
	}
	//��
	for(int j=0 ; j<=5 ; j++)
	{
		for (int i = 0; i< height ; i++)
		{
			int channel0 = (uchar)(gray->imageData + i*gray->widthStep)[j*gray->nChannels + 0];
			if (channel0 == 0)
			{
				int channel0 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 0];
				int channel1 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 1];
				int channel2 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 2];
				if (channel0 < 100 && channel1 < 100 && channel2 < 100)
				{
					//cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(5,5,5,0),NULL,4);
					cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(250,250,250,0),NULL, 4);
				}
			}
		}
	}
	//��
	for (i = height-1 ; i>=height-5 ; i--)
	{
		for (int j = 0; j< width ; j++)
		{
			int channel0 = (uchar)(gray->imageData + i*gray->widthStep)[j*gray->nChannels + 0];
			if (channel0 == 0)
			{
				int channel0 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 0];
				int channel1 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 1];
				int channel2 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 2];
				if (channel0 < 100 && channel1 < 100 && channel2 < 100)
				{
					//cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(5,5,5,0),NULL,4);
					cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(250,250,250,0),NULL, 4);
				}
			}
		}
	}
	//��
	for(j=width-1 ; j>=width-5 ; j--)
	{
		for (int i = 0; i< height ; i++)
		{
			int channel0 = (uchar)(gray->imageData + i*gray->widthStep)[j*gray->nChannels + 0];
			if (channel0 == 0)
			{
				int channel0 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 0];
				int channel1 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 1];
				int channel2 = (uchar)(img_src->imageData + i*img_src->widthStep)[j*img_src->nChannels + 2];
				if (channel0 < 100 && channel1 < 100 && channel2 < 100)
				{
					//cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(5,5,5,0),NULL,4);
					cvFloodFill(img_src,cvPoint(j,i),cvScalar(255,255,255),cvScalar(5,5,5,0),cvScalar(250,250,250,0),NULL, 4);
				}
			}
		}
	}
} 

void OpenCVImageHandle::cv_RemoveBorder(string inPath, string outPath)
{
	IplImage* img_src = cvLoadImage(inPath.c_str(), 1);

	removeblack(img_src);	

	cvSaveImage(outPath.c_str(),img_src);
	cvReleaseImage(&img_src);
} 

BOOL OpenCVImageHandle::cv_isAllWhite(string inPath)
{
	IplImage* img = cvLoadImage(inPath.c_str(), 1);

	IplImage* gray = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor( img, gray, CV_BGR2GRAY);

	int r,g,b;
	int nNonWhiteCount = 0;

	// �ٷֱ���ֵ��Ϊ0.005(0.5%)
	float nThreshold = 0.005;
	int nThresholdCount = img->width * img->height * nThreshold;

	for(int x = 0; x < img->width; x++)
	{
		for(int y = 0; y < img->height; y++)
		{
			r = CV_IMAGE_ELEM(img, uchar, y, x * 3 + 2);
			g = CV_IMAGE_ELEM(img, uchar, y, x * 3 + 1);
			b = CV_IMAGE_ELEM(img, uchar, y, x * 3);

			// ������ҪѰ�ҵ�һ�����ʵ���ֵ��������������
			// 1��ʲô��Χ����ɫ���ڰ�ɫ
			// 2����Ϊ�ǿհ�ҳ�Ĳ���ɫ�İٷֱȶ�Ϊ���ٱȽϺ��ʣ�
			if(r >= 200 && g >= 200 && b >= 200)
			{
				continue;
			}
			else
			{	
				nNonWhiteCount++;
				if(nNonWhiteCount > nThresholdCount)
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void OpenCVImageHandle::trackbar(int pos)
{
	printf("Trackbar position:%d\n",pos);
}

// !˳ʱ��ͼƬ��ת
IplImage* OpenCVImageHandle::rotateImage(IplImage* src, int angle, bool clockwise)  
{  
	angle = abs(angle) % 180;  
	if (angle > 90)  
	{  
		angle = 90 - (angle % 90);  
	}  
	IplImage* dst = NULL;  
	int width =  
		(double)(src->height * sin(angle * CV_PI / 180.0)) +  
		(double)(src->width * cos(angle * CV_PI / 180.0 )) + 1;  
	int height =  
		(double)(src->height * cos(angle * CV_PI / 180.0)) +  
		(double)(src->width * sin(angle * CV_PI / 180.0 )) + 1;  
	int tempLength = sqrt((double)src->width * src->width + src->height * src->height) + 10;  
	int tempX = (tempLength + 1) / 2 - src->width / 2;  
	int tempY = (tempLength + 1) / 2 - src->height / 2;  
	int flag = -1;  

	dst = cvCreateImage(cvSize(width, height), src->depth, src->nChannels);  
	cvZero(dst);  
	IplImage* temp = cvCreateImage(cvSize(tempLength, tempLength), src->depth, src->nChannels);  
	cvZero(temp);  

	cvSetImageROI(temp, cvRect(tempX, tempY, src->width, src->height));  
	cvCopy(src, temp, NULL);  
	cvResetImageROI(temp);  

	if (clockwise)  
		flag = 1;  

	float m[6];  
	int w = temp->width;  
	int h = temp->height;  
	m[0] = (float) cos(flag * angle * CV_PI / 180.);  
	m[1] = (float) sin(flag * angle * CV_PI / 180.);  
	m[3] = -m[1];  
	m[4] = m[0];  
	// ����ת��������ͼ���м�  
	m[2] = w * 0.5f;  
	m[5] = h * 0.5f;  
	//  
	CvMat M = cvMat(2, 3, CV_32F, m);  
	cvGetQuadrangleSubPix(temp, dst, &M);  
	cvReleaseImage(&temp);  
	return dst;  
} 

// !ͼƬ˳ָ����ת��ʮ��
void OpenCVImageHandle::cv_rotateImage(string inPath, string outPath)
{
	IplImage *src = 0;  
	IplImage *dst = 0;  
	// ��ת�Ƕ�  
	int angle = 90;  

	src = cvLoadImage(inPath.c_str(), CV_LOAD_IMAGE_COLOR);  
	dst = rotateImage(src, angle, true);  
	
	cvSaveImage(outPath.c_str(),dst);
	cvReleaseImage(&src);  
	cvReleaseImage(&dst);  

}

//  !ͼƬ����
void OpenCVImageHandle::cv_PictureCutting(string picPath, string outPath, int x, int y, int width, int hight)
{
	IplImage * cutimage = cvLoadImage(picPath.c_str(),1);
	//cvSetImageROI(cutimage,cvRect(0.68*cutimage->width,0.13*cutimage->height,0.075*cutimage->width,0.15*cutimage->height));
	//IplImage * dst = cvCreateImage(cvSize(0.075*cutimage->width,0.15*cutimage->height),cutimage->depth,cutimage->nChannels);
	CvRect rect;
	rect.x =x;
	rect.y = y;
	rect.width = width;
	rect.height = hight;

	// 	double dwFloat = (double)3507/585;
	// 	rect.x = 48*dwFloat;
	// 	rect.y = 61*dwFloat;
	// 	rect.width = 322*dwFloat;
	// 	rect.height = 459*dwFloat;
	//cvSetImageROI(cutimage,cvRect(100,100,cutimage->width-200,cutimage->height-200));

	cvSetImageROI(cutimage,rect);
	IplImage * dst = cvCreateImage(cvSize(rect.width,rect.height),cutimage->depth,cutimage->nChannels);
	cvCopy(cutimage,dst,0);
	//cvSaveImage("F:\\image\\dbzhengquan\\0605_0001.jpg",dst);
	cvSaveImage(outPath.c_str(),dst);
	cvResetImageROI(cutimage);
	printf("cut over");
}

// ��ֵ��
void OpenCVImageHandle::cv_PictureBinary(string picPath, string picSavePath)
{
	IplImage* src1 = cvLoadImage(picPath.c_str(),1);
	IplImage* gray1 = cvCreateImage( cvGetSize(src1),8,1);
	cvCvtColor(src1, gray1, CV_BGR2GRAY);            //ת��Ϊ��ֵͼ��
	cvThreshold(gray1, gray1,110,255,CV_THRESH_BINARY);//��ֵ��

	/*int  pg[256];
	memset(pg,0, 256*sizeof(int));
	for (int i=0; i<gray->imageSize; i++)      //  ֱ��ͼͳ��
	pg[(unsigned char)gray->imageData[i]]++;
	int thre = BasicGlobalThreshold(pg, 0, 256);    //  ȷ����ֵ
	cvThreshold(gray, gray, thre, 255, CV_THRESH_BINARY);  //  ��ֵ��
	cvSmooth(gray,gray,CV_MEDIAN,3,0,0,0); 
	cvErode(gray,gray,0,2);                       //��ʴ
	cvDilate(gray,gray,0,1); 
	*/

	cvSaveImage(picSavePath.c_str(),gray1);
}

void OpenCVImageHandle::cv_FillWhite(string inPath, string outPath, string templatewhitePath, int x, int y, int width, int hight)
{
	//IplImage* img = cvLoadImage("F:\\image\\dbzhengquan\\template01.jpg",0);      
	IplImage* img = cvLoadImage(inPath.c_str(),0);      

	CvRect roi =cvRect(x,y,width,hight);   

	//IplImage* img1 = cvLoadImage("F:\\image\\dbzhengquan\\template_white.jpg",0);     
	IplImage* img1 = cvLoadImage(templatewhitePath.c_str(),0);     

	cvSetImageROI(img, roi);  
	cvSetImageROI(img1, roi);  
	cvCopy(img1, img);  
	cvResetImageROI(img);  
	cvResetImageROI(img1);  

	cvSaveImage(outPath.c_str(),img);

	cvReleaseImage(&img);  
	cvReleaseImage(&img1);  
}

int OpenCVImageHandle::HoughLines2(IplImage * source,IplImage * src, string outPath)
{
	//const char* filename = "C:\\DB_test\\038\\Image_004.jpg";
	//IplImage* src = cvLoadImage( filename, 0 );
	IplImage* dst;
	IplImage* color_dst;
	IplImage* srctemp;
	float PI = 3.141592653589793 ;
	int i=0;  // ͳ����б�Ƕ�>0�Ĵ���

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	vector<float> vec_float;
	vector<int> vec_rightnumber;
	vector<int> vec_leftnumber;
	vector<int> vec_oknumber;
	int iCountRight = 0;  //����ת������
	int iCountLeft = 0;	// ��ѡװ������
	int iCountOk = 0;  // �պò�ѡװ��������

	if( !src )
		return -1;

	srctemp = cvCreateImage( cvGetSize(source),IPL_DEPTH_8U, 1 );
	dst = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	color_dst = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 3 );

	// Snail���������´��룺cvCannyֻ���ܻҶ�ͼ������Ĳ�ɫͼ������붪ʧ��ɫ��
	// ���Ƚ�������ת�����������cvCanny֮����ת���������ɡ�
	IplImage* gray_src = cvCreateImage( cvGetSize(source),IPL_DEPTH_8U, 1 );
	cvCvtColor( src, gray_src, CV_BGR2GRAY);
	/////////////////////////////////////////////////////////////////////////
	cvCanny( gray_src, dst, 50, 200, 3 );
	cvCvtColor( dst, color_dst, CV_GRAY2BGR );
#if 1
	lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

	for( i = 0; i < MIN(lines->total,100); i++ )
	{
		float* line = (float*)cvGetSeqElem(lines, i);
		float rho = line[0];
		float theta = line[1];
		double dFloat;
		CvPoint pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		
		//cout << "pt1.x =" << pt1.x << " pt1.y = " << pt1.y << endl;
		//cout << "pt2.x =" << pt2.x << " pt2.y = " << pt2.y << endl;

		//dFloat = (double)(pt1.y - pt2.y)/(pt1.x-pt2.x);

		if(abs(pt1.y) == abs(pt2.y) || abs(pt1.x) == abs(pt2.x))  // ���ߺ�����
		{
			iCountOk++;
			vec_oknumber.push_back(i);
		}
		else if (pt1.y<pt2.y)
		{
			iCountRight++;
			vec_rightnumber.push_back(i);
		}
		else
		{
			iCountLeft++;
			vec_leftnumber.push_back(i);
		}
		float thetia = atan2( float(pt1.y-pt2.y), float(pt1.x-pt2.x) );
		float angle0 = 180 - abs(thetia*180/PI);
		vec_float.push_back(angle0);

		cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );
		
		//cvSaveImage("C:\\DB_test\\038\\001\\temp.jpg",color_dst);
	}
#else
	lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );
	for( i = 0; i < lines->total; i++ )
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
		cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
	}
#endif

	int iMax = Max(iCountRight,iCountLeft,iCountOk);
	/************************************
	* 1 ��ȷ������Ҫ���ƫ�����Ҿ�ƫ
	* 2 ȷ��OK ����Ҫ��ƫ�Ķ���
	* 3 ���з��Ͼ�ƫ����ĽǶȶ�����vector<>��
	* 4 ����Vector<> �ҳ�һ�� ����ʵĽǶ�
	* 5 ����ʵĽǶ� ���� ����Ƕ�b�Ǹ�vector�е�����,�����Vector<>����b��Ȼ��߲�ֵС�ڵ���0.5�ȵ���ֵ���
	*   ��ôȷ��bΪҪ��ת�ĽǶ�*
	*************************************/

	float fLeteRight = FindMaxCountValue(vec_float);
	if (iMax == iCountRight)   // ��Ҫ���ƫ ��ʱ��תͼ
	{
		/*dst = cvCreateImage( cvGetSize(src), 8, 1 );*/
		srctemp = rotateImage(source,fLeteRight,false);
		
		//cvSaveImage("C:\\test.tif",srctemp);
		//srctemp = RoteImg(source,fLeteRight);
		
		//Snail �޸�
		//cv_RemoveBorder("C:\\test.tif", "C:\\test_removeblack.tif");
		removeblack(srctemp);
		//srctemp = cvLoadImage("C:\\test_removeblack.tif", 1);
			
		if (srctemp->width > src->width && srctemp->height > src->height)
		{
			CvRect rect;
			rect.x = (srctemp->width-src->width)/2;
			rect.y = (srctemp->height-src->height)/2;
			rect.width = src->width;
			rect.height = src->height;
			cvSetImageROI(srctemp,rect);
			IplImage * dsttemp = cvCreateImage(cvSize(rect.width,rect.height),srctemp->depth,srctemp->nChannels);
			cvCopy(srctemp,dsttemp,0);
			//cvSaveImage("F:\\image\\dbzhengquan\\0605_0001.jpg",dst);
			cvSaveImage(outPath.c_str(),dsttemp);
			cvResetImageROI(srctemp);
			cvReleaseImage(&dsttemp);

		}
		else
			cvSaveImage(outPath.c_str(),srctemp);


	}
	else if (iMax == iCountLeft)
	{
		srctemp = rotateImage(source,fLeteRight,true);
		//srctemp = RoteImg(source,-fLeteRight);
		//cvSaveImage("D:\\test.tif",srctemp);

		removeblack(srctemp);
		
		if (srctemp->width > src->width && srctemp->height > src->height)
		{
			CvRect rect;
			rect.x = (srctemp->width-src->width)/2;
			rect.y = (srctemp->height-src->height)/2;
			rect.width = src->width;
			rect.height = src->height;
			cvSetImageROI(srctemp,rect);
			IplImage * dsttemp = cvCreateImage(cvSize(rect.width,rect.height),srctemp->depth,srctemp->nChannels);
			cvCopy(srctemp,dsttemp,0);
			//cvSaveImage("F:\\image\\dbzhengquan\\0605_0001.jpg",dst);
			cvSaveImage(outPath.c_str(),dsttemp);
			cvResetImageROI(srctemp);
			cvReleaseImage(&dsttemp);

		}
		else
			cvSaveImage(outPath.c_str(),srctemp);

	}
	else
	{
		return -1;
	}


	//cvNamedWindow( "Source", 1 );
	//cvShowImage( "Source", src );

	//cvNamedWindow( "Hough", 1 );
	//cvShowImage( "Hough", color_dst );

	//cvWaitKey(0);
	cvReleaseImage(&dst);
	cvReleaseImage(&color_dst);
	cvReleaseImage(&srctemp);

	//system("pause");
	return 0;
}

void OpenCVImageHandle::cv_roteImgSelf(string inPath, string outPath, int& nRetValue)
{
	IplImage *img = cvLoadImage(inPath.c_str(),1);		//Ϊʲô������1? ������δ����ɫͼƬ(Snail)
	if(img == NULL)  
	{  
		printf("img load failed!\n");  
		return;  
	}  
	IplImage *img_erode = cvCreateImage(cvGetSize(img), img->depth,img->nChannels);  
	//IplImage *img_dilate = cvCreateImage(cvGetSize(img), 8, 1);  

	cvErode( img,img_erode, NULL,20); //��ʴ   
	//cvSaveImage("C:\\DB_test\\038\\001\\Image_00003_erode.jpg",img_erode);
	//cvDilate( img,img_dilate, NULL,20); //����   
	nRetValue = HoughLines2(img,img_erode,outPath);

	//cvSaveImage("C:\\DB_test\\038\\Image_00003_erode.jpg",img_erode);
	//cvSaveImage("C:\\DB_test\\038\\Image_00003_dilate.jpg",img_dilate);

	//cvNamedWindow("img_erode");  
	//cvNamedWindow("img_dilate");  

	//cvShowImage("img_erode",img_erode);  
	//cvShowImage("img_dilate",img_dilate);  

	//cvWaitKey(-1);  


	//cvReleaseImage(&img_dilate);  
	cvReleaseImage(&img_erode);  
	cvReleaseImage(&img);

	//cvDestroyAllWindows();   
}