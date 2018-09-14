#include <cstdlib>
#include <algorithm>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <highgui.h>
#include "cv.h"
#include <fstream>
using namespace std;
using namespace cv;
Mat Pseudoinverse(Mat x);
Mat Mynormalized(Mat x);
void mifilter(Mat x);
uchar sort(uchar in[9]);

void Printmat(Mat x);
class point {
public:
	int x;
	int y;
	int z;
};