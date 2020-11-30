#include "DataContour.h"


Rect DataContour::getBoundingRect()
{
	return boundingRect;
}

vector<Point> DataContour::getPoints()
{
	return points;
}

float DataContour::getArea()
{
	return area;
}

DataContour::DataContour(vector<Point> contour)
{
		this->points = contour;
		this->boundingRect = cv::boundingRect(contour);
		this->area= contourArea(contour);
}

bool DataContour::checkIfContourIsValid() {                              // obviously in a production grade program
	if (area < MIN_CONTOUR_AREA)
		return false;           // we would have a much more robust function for 
	return true;                                            // identifying if a contour is valid !!
}

 bool DataContour::sortByBoundingRectXPosition(const DataContour& cwdLeft, const DataContour& cwdRight) {      // this function allows us to sort
	return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
}