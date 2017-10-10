#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include "iostream"
using namespace cv;
using namespace std;

int histogram(Mat img,Mat red_histogram,Mat green_histogram,Mat blue_histogram);
int main()
{
Mat img=imread("test_image_contrast_2.jpg");
Mat color(img.rows,img.cols,CV_8UC3);
Mat red(img.rows,img.cols,CV_8UC1);
Mat blue(img.rows,img.cols,CV_8UC1);
Mat green(img.rows,img.cols,CV_8UC1);
for(int i=0;i<img.rows;i++)
{
	for(int j=0;j<img.cols;j++)
	{
		blue.at<uchar>(i,j)=img.at<Vec3b>(i,j)[0];
		red.at<uchar>(i,j)=img.at<Vec3b>(i,j)[2];
		green.at<uchar>(i,j)=img.at<Vec3b>(i,j)[1];
	}
}
equalizeHist(red,red);
equalizeHist(blue,blue);
equalizeHist(green,green);
for(int i=0;i<img.rows;i++)
{
	for(int j=0;j<img.cols;j++)
	{
		color.at<Vec3b>(i,j)[0]=blue.at<uchar>(i,j);
		color.at<Vec3b>(i,j)[2]=red.at<uchar>(i,j);
		color.at<Vec3b>(i,j)[1]=green.at<uchar>(i,j);
	}
}
int r=110;
Mat red_histogram_original(r,256,CV_8UC3,Scalar(0,0,0));
Mat blue_histogram_original(r,256,CV_8UC3,Scalar(0,0,0));
Mat green_histogram_original(r,256,CV_8UC3,Scalar(0,0,0));
Mat red_histogram_result(r,256,CV_8UC3,Scalar(0,0,0));
Mat blue_histogram_result(r,256,CV_8UC3,Scalar(0,0,0));
Mat green_histogram_result(r,256,CV_8UC3,Scalar(0,0,0));
histogram(img,red_histogram_original,green_histogram_original,blue_histogram_original);
histogram(color,red_histogram_result,green_histogram_result,blue_histogram_result);
imshow("red_histogram_original",red_histogram_original);
imshow("green_histogram_original",green_histogram_original);
imshow("blue_histogram_original",blue_histogram_original);
imshow("red_histogram_result",red_histogram_result);
imshow("green_histogram_result",green_histogram_result);
imshow("blue_histogram_result",blue_histogram_result);
imshow("original",img);
imshow("result",color);
waitKey(0);
return 0;
}
int histogram(Mat img,Mat red_histogram,Mat green_histogram,Mat blue_histogram)
{
	float f_red[256],f_blue[256],f_green[256];
	float n_pixels=0;
	for(int i=0;i<256;i++)
	{
		f_red[i]=0.0;
		f_green[i]=0.0;
		f_blue[i]=0.0;
	}
	for(int i=0;i<img.rows;i++)
	{
		for(int j=0;j<img.cols;j++)
		{
			f_red[img.at<Vec3b>(i,j)[2]]++;
			f_blue[img.at<Vec3b>(i,j)[0]]++;
			f_green[img.at<Vec3b>(i,j)[1]]++;
			n_pixels++;
		}
}	
float f_red_max=0,f_green_max=0,f_blue_max=0;
for(int i=0;i<256;i++)
{
	if(f_red_max<f_red[i])
		f_red_max=f_red[i];
	if(f_green_max<f_green[i])
		f_green_max=f_green[i];
	if(f_blue_max<f_blue[i])
		f_blue_max=f_blue[i];
}
for(int i=0;i<256;i++)
{
	f_red[i]=(int)(f_red[i]/(f_red_max/100));
	f_green[i]=(int)(f_green[i]/(f_green_max/100));
	f_blue[i]=(int)(f_blue[i]/(f_blue_max/100));
}
for(int i=0;i<256;i++)                                                                                                                
{
	for(int j=1;j<=f_red[i];j++)
		red_histogram.at<Vec3b>(red_histogram.rows-j,i)[2]=255;
	for(int j=1;j<=f_green[i];j++)	
		green_histogram.at<Vec3b>(green_histogram.rows-j,i)[1]=255;
	for(int j=1;j<=f_blue[i];j++)
		blue_histogram.at<Vec3b>(blue_histogram.rows-j,i)[0]=255;
}
return 0;
}