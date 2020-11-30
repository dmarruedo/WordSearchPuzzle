#include "_Contour.h"

_Contour::_Contour(vector<Point2f> contour)
{
	this->contour = contour;
}

void _Contour::setHeight(int heigth)
{
	this->height = height;
}

void _Contour::setWidth(int width)
{
	this->width = width;
}

void _Contour::setResolution(int resolution)
{
	this->resolution = resolution;
}

int _Contour::getHeigth()
{
	return height;
}

int _Contour::getWidth()
{
	return width;
}

int _Contour::getResolution()
{
	return resolution;
}

vector<Point2f> _Contour::getContour()
{
	return points;
}
