#include "WordSearchImage.h"
#include "ImageContour.h"





WordSearchImage::WordSearchImage(Mat originalImage)
{
	this->originalImage = originalImage;
}

void WordSearchImage::setContour(ImageContour contour)
{
	this->contour = contour;
}

ImageContour WordSearchImage::getContour()
{
	return contour;
}

void WordSearchImage::setRows(int rows)
{
	this->rows = rows;
}

void WordSearchImage::setCols(int cols)
{
	this->cols = cols;
}

int WordSearchImage:: getRows()
{
	return rows;
}

int WordSearchImage::getCols()
{
	return cols;
}


ImageContour WordSearchImage::findContour(bool SHOWIMAGE)
{
	vector<vector<Point2f> > contoursPoint;
	findContours(originalImage, contoursPoint, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	double area; double maxarea = 0;  int p; 

	for (int i = 0; i < contoursPoint.size(); i++)
	{
		area = contourArea(contoursPoint[i], false);
		if (area > 25)
		{
			if (area > MIN_AREA)
			{
				maxarea = area;
				p = i;
			}
			
		}
	}

	double perimeter;

	// Se aproxima el contorno a un poligono de cuatro vertices para el contorno exterior
	perimeter = arcLength(contoursPoint[p], true);
	approxPolyDP(contoursPoint[p], contoursPoint[p], 0.01*perimeter, true);

	setContour(contoursPoint[p]);

	if (SHOWIMAGE){
		Mat contourImage;
		cvtColor(originalImage, contourImage, COLOR_GRAY2BGR);

		drawContours(contourImage, contoursPoint, p, Scalar(255, 0, 0), 1, 8);
		namedWindow("Contorno exterior", CV_WINDOW_AUTOSIZE);
		imshow("Contorno exterior", contourImage);
		waitKey(0);
		destroyAllWindows();
	}
	ImageContour contour(contoursPoint[p]);


	contour = contour.clockwiseOrderVertix();
	contour.calculateDimensions();
	setContour(contour);

	return contour;
}



Mat WordSearchImage::getPerspectiveSearchImage(void)
{
	return Mat();
}
