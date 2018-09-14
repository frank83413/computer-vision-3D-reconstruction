#include <cstdlib>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <highgui.h>
#include "cv.h"
#include "function.h";
#include <fstream>
#include <cmath>
using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
	float finalz = 0, tempfinalz = 0;
	int lightcount=0,tempx=0,tempy,tempz,drop,count=0;
	Mat light=Mat(6, 3, CV_32FC1);
	Mat lightin= Mat(3, 6, CV_32FC1);
	Mat finalpoint;
	Mat out;
	char templine[50];
	Mat pic[6],picall,picallfloat,norxyz;
	//讀light
	fstream fi;
	fi.open("test/bunny/LightSource.txt", ios::in);
	while (fi.getline(templine, sizeof(templine), '\n')) {
		sscanf(templine,"pic%d: (%d, %d, %d)" , &drop, &tempx,&tempy,&tempz);
		light.at<float>(lightcount, 0) = tempx;
		light.at<float>(lightcount, 1) = tempy;
		light.at<float>(lightcount, 2) = tempz;
		//cout << light.at<float>(lightcount, 2) << "\n";
		lightcount++;
	}
	//讀圖 若要使用other的圖片請記得改成.jpg格式
	pic[0] = imread("test/bunny/pic1.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	pic[1] = imread("test/bunny/pic2.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	pic[2] = imread("test/bunny/pic3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	pic[3] = imread("test/bunny/pic4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	pic[4] = imread("test/bunny/pic5.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	pic[5] = imread("test/bunny/pic6.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	for (int i = 0; i < 6; i++) {
		mifilter(pic[i]);	//對影像先做去雜訊若非special則可註解掉
	}

	finalpoint = Mat(pic[0].rows,pic[0].cols,CV_32FC1);
	picall = Mat(6,pic[0].rows*pic[0].cols, CV_8UC1);
	picallfloat = Mat(6, pic[0].rows*pic[0].cols, CV_32FC1);
	norxyz = Mat(3, pic[0].rows*pic[0].cols, CV_32FC1);

	//將6張圖從入同一個陣列
	for (int i = 0,temp=0; i < 6;i++) {
		temp = 0;
		for (int j = 0; j < pic[0].rows; j++) {
			for (int k = 0; k < pic[0].cols;k++ ) {
				picall.at<uchar>(i, temp) = pic[i].at<uchar>(j, k);
				temp++;
			}
		}
	}

	
	lightin = Pseudoinverse(light);
	picall.convertTo(picallfloat , CV_32FC1, 1.0/255.0);//轉為float相乘
	norxyz = lightin*picallfloat;
	norxyz=Mynormalized(norxyz);

	count = -1;
	//Printmat(norxyz);
	for (int i = 0; i < finalpoint.rows; i++) {
		finalz = 0;
		finalpoint.at<float>(i, 0) = finalz;
		count++;
		for (int j = 1; j < finalpoint.cols; j++) {			
			if (norxyz.at<float>(2, count) != 0) {
				//如果兩邊normal差太大則不修改
				if (norxyz.at<float>(0, count)/ norxyz.at<float>(2, count) - norxyz.at<float>(0, count - 1)/ norxyz.at<float>(2, count) > 1) {
					finalpoint.at<float>(i, j) = finalz;
					count++;
				}
				else {
					finalz = finalz*norxyz.at<float>(2, count) - (norxyz.at<float>(0, count)) / (norxyz.at<float>(2, count));//znew=zold-nx/nz->全部從右積到左
					finalpoint.at<float>(i, j) = finalz;
					count++;
				}
			}
			else
			{
				finalpoint.at<float>(i, j) = 0;
				count++;
			}
		}
	}


	count = 0;
	out = Mat(pic[0].rows, pic[0].cols, CV_8UC3);
	//顯示各pixel之normal
	for (int i = 0; i < out.rows; i++) {
		for (int j = 0; j < out.cols; j++) {			
			out.at<Vec3b>(i, j)[0] = 255*norxyz.at<float>(0, count);
			out.at<Vec3b>(i, j)[1] = 255*norxyz.at<float>(1, count);
			out.at<Vec3b>(i, j)[2] = 255*norxyz.at<float>(2, count);
				count++;
		}
	}
	
	//Printmat(finalpoint);
	//outputfile
	//alpha值 bunny=1,star=2,venus=1,other=3
	ofstream outfile("test/bunny-surface.ply");
	outfile <<
		"ply\n""format ascii 1.0\n""comment alpha=1.0\n";
	outfile << "element vertex " << pic[0].rows * pic[0].cols << "\n";
	outfile <<
		"property float x\n""property float y\n""property float z\n""property uchar red\n"
		"property uchar green\n""property uchar blue z\n""end_header\n"
		;
	for (int i=0; i < pic[0].rows; i++) {
		for (int j = 0; j < pic[0].cols; j++) {			
			if(finalpoint.at<float>(i, j) ==0)outfile << i << ' ' << j << ' ' << finalpoint.at<float>(i, j) << " 255 255 255\n";
			else outfile << i << ' ' << j << ' ' << finalpoint.at<float>(i, j) << " 255 000 000\n";			
		}
	}
	outfile.close();


	cv::imshow("123", out);//輸出normal圖
	//system("pause");
	cv::waitKey();
}

