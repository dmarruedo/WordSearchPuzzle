#pragma once
#include "opencv\cv.hpp"
#include "ImageContour.h"
#include "DataContour.h"
#include<opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;


vector<ImageContour> findWordSearhContours(Mat image, bool SHOW_IMAGE);
int calculateRows(ImageContour wordSearchContour, ImageContour cellContour, double error);
int calculateCols(ImageContour wordSearchContour, ImageContour cellContour, double error);
Mat extractCellImage(Mat wordSearchImage, int row, int col, double cellWidth, double cellHeight, bool SHOW_IMAGE);
Mat cutOnPerspective(Mat image, ImageContour contour , bool SHOW_IMAGE);
char OCR(String image, bool SHOW_IMAGE);