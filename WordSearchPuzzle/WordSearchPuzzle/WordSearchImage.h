#pragma once

#include <iostream>
#include "opencv\cv.hpp"
#include "ImageContour.h"


using namespace cv;
using namespace std;
using namespace ImageContour;


class WordSearchImage
{
private:
	Mat originalImage;
	Mat perpectiveWordSearchImage;
	int rows;
	int cols;
	vector<vector<Mat>> celdas;
	ImageContour contour;
	int MIN_AREA = 100;
	

	void setContour(ImageContour contour);

public:
	WordSearchImage(Mat originalImage);

	void setRows(int rows);
	void setCols(int cols);
	int getRows();
	int getCols();
;
	ImageContour getContour();
	
	ImageContour findContour(bool SHOWIMAGE);

	

	Mat getPerspectiveSearchImage(void);





};

