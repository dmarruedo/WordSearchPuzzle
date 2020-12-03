#pragma once
#include "opencv\cv.hpp"
#include "ImageContour.h"
#include "DataContour.h"
#include<opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

const int RESIZED_IMAGE_WIDTH = 20;
const int RESIZED_IMAGE_HEIGHT = 30;
const int MIN_CELL_AREA = 400;

struct WordPos
{
	int Xpos;
	int Ypos;
	int Xdir;
	int Ydir;
	int wordSize;

};

vector<ImageContour> findWordSearhContours(Mat image, bool SHOW_IMAGE);
int calculateRows(ImageContour wordSearchContour, ImageContour cellContour, double error);
int calculateCols(ImageContour wordSearchContour, ImageContour cellContour, double error);
Mat extractCellImage(Mat wordSearchImage, ImageContour cellContour, bool SHOW_IMAGE);
Mat extractCellImage(Mat wordSearchImage, int row, int col, double cellWidth, double cellHeight, bool SHOW_IMAGE);
Mat cutOnPerspective(Mat image, ImageContour contour , bool SHOW_IMAGE);
Ptr<cv::ml::KNearest> loadOCR();
char OCR(Mat image, Ptr<cv::ml::KNearest> KNearest,  bool SHOW_IMAGE);
Mat roundWord(Mat roundsImage, vector<vector<ImageContour>> contourCellsMatriz, vector<Point> solution, bool DEBUG_MODE);
Mat roundWord(Mat roundsImage, vector<Point> solution, int rows, int cols, double cellWidth, double cellHeight, bool DEBUG_MODE);
vector<vector<ImageContour>> orderCellContour(ImageContour extContour, vector<ImageContour> cellContours);
struct WordPos WordSearch(vector<vector<char>>sopaLetras, int columns, int rows);