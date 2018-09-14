#include <cstdlib>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <highgui.h>
#include "cv.h"
#include <fstream>
#include "function.h"

using namespace std;
using namespace cv;

//求light的inverse
// [L(T)*L].inv*L(T)
Mat Pseudoinverse(Mat x){

Mat transposex = Mat(x.cols, x.rows, CV_32FC1);
Mat xmultranspose = Mat(x.cols, x.cols, CV_32FC1);
transpose(x, transposex);
xmultranspose = transposex*x;
xmultranspose = xmultranspose.inv();
return xmultranspose*transposex;
}

//將向量normalize vector(x,y,z)/(x^2+y^2+z^2)^0.5
//向量對應到傳入Mat的col
Mat Mynormalized(Mat x) {
	float tempx, tempy, tempz,tempnor;
	Mat temp=Mat(x.rows,x.cols, CV_32FC1);
	for (int i = 0; i < x.cols; i++) {
		tempx = x.at<float>(0, i);
		tempy = x.at<float>(1, i);
		tempz = x.at<float>(2, i);
		tempx = tempx*tempx;
		tempy = tempy*tempy;
		tempz = tempz*tempz;
		tempnor = sqrt(tempx + tempy + tempz);
		if (tempnor == 0)continue;
		temp.at<float>(0, i) = x.at<float>(0, i) / tempnor;
		temp.at<float>(1, i) = x.at<float>(1, i) / tempnor;
		temp.at<float>(2, i) = x.at<float>(2, i) / tempnor;
	}
	return temp;
}

//印出Mat以測試
void Printmat(Mat x) {
	for (int i = 0; i < x.rows; i++) {
		for (int j = 0; j < x.cols; j++) {
			cout << x.at<float>(i, j) << ",";
		}
		cout << "\n";
	}
}

//採取中間值濾波器消除雜訊
void mifilter(Mat x) {
	uchar sortarray[9];
	for (int i = 0; i < x.cols; i++)
	{
		x.at<uchar>(0, i) = 0;
		x.at<uchar>(x.rows-1, i) = 0;
	}
	for (int i = 0; i < x.rows; i++)
	{
		x.at<uchar>(i, 0) = 0;
		x.at<uchar>(i, x.cols-1) = 0;
	}

	for (int i = 1; i < x.rows-1; i++)
	{
		for (int j = 1; j < x.cols-1; j++) {
			sortarray[0] = x.at<uchar>(i-1, j-1);
			sortarray[1] = x.at<uchar>(i-1, j);
			sortarray[2] = x.at<uchar>(i-1, j+1);
			sortarray[3] = x.at<uchar>(i, j-1);
			sortarray[4] = x.at<uchar>(i, j);
			sortarray[5] = x.at<uchar>(i, j+1);
			sortarray[6] = x.at<uchar>(i+1, j-1);
			sortarray[7] = x.at<uchar>(i+1, j);
			sortarray[8] = x.at<uchar>(i+1, j+1);
			x.at<uchar>(i,j)=sort(sortarray);
		}
	}
}
//排序取中間值
uchar sort(uchar in[9]) {
	uchar temp;
	for (int i = 0; i < 9;i++) {
		for (int j = 0; j < 9 - i;j++) {
			if (in[j] > in[j + 1]) {
				temp = in[j];
				in[j] = in[j + 1];
				in[j + 1] = temp;
			}
		}
	}
	return in[4];
}