#include "ImageContour.h"

ImageContour::ImageContour()
{
}

ImageContour::ImageContour(vector<Point> contour)
{

	double perimeter;

	
	perimeter = arcLength(contour, true);
	approxPolyDP(contour, contour, 0.1*perimeter, true);
	
	this->points= clockwiseOrderVertix(contour);
	this->height = calculateHeight(this->points);
	this->width = calculateWidth(this->points);


}




vector<Point> ImageContour::getPoints() {
	return points;
}

double ImageContour::getWidth()
{
	return width;
}

double ImageContour::getHeight()
{
	return height;
}




int ImageContour::calculateWidth(vector<Point> contour)
{
	double width1; double width2;
	width1 = contour[1].x - contour[0].x;
	width2 = contour[2].x - contour[3].x;
	return ((width1 + width2) / 2);
}

int ImageContour::calculateHeight(vector<Point> contour)
{
	double heigth1; double heigth2;

	heigth1 = contour[3].y - contour[0].y;
	heigth2 = contour[2].y - contour[1].y;
	return ((heigth1 + heigth2) / 2);
}




vector<Point> ImageContour::clockwiseOrderVertix(vector<Point> contour)
{
	double sum;
	double prevsum;

	int top_right; int bottom_left; int bottom_right; int top_left;
	// codigo para asignar los vertices encontrados del cuadrado exterior a su posicion en el plano (top-Left , top_right , bottom_right , bottom_left)
	prevsum = contour[0].x + contour[0].y;
	bottom_right = 0;
	for (int i = 0; i < 4; i++)
	{
		sum = contour[i].x + contour[i].y;
		if (sum > prevsum)
		{
			prevsum = sum;
			bottom_right = i;
		}
	}
	prevsum = contour[0].x + contour[0].y;
	top_left = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contour[i].x + contour[i].y;
		if (sum <= prevsum)
		{
			prevsum = sum;
			top_left = i;
		}
	}
	prevsum = contour[0].x - contour[0].y;
	bottom_left = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contour[i].x - contour[i].y;
		if (sum <= prevsum)
		{
			prevsum = sum;
			bottom_left = i;
		}
	}

	prevsum = contour[0].x - contour[0].y;
	top_right = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contour[i].x - contour[i].y;
		if (sum > prevsum && i != top_left)
		{
			prevsum = sum;
			top_right = i;
		}
	}

	vector<Point> orderContour;
	orderContour.push_back(contour[top_left]);
	orderContour.push_back(contour[top_right]);
	orderContour.push_back(contour[bottom_right]);
	orderContour.push_back(contour[bottom_left]);


	return orderContour;

}
