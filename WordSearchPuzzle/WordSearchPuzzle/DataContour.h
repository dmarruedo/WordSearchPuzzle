#pragma once

#include "opencv\cv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// variables globales
const int MIN_CONTOUR_AREA = 50;


class DataContour
{

	private:
		vector<Point> points;					// contour
		Rect boundingRect;							// bounding rect for contour
		float area;                              // area of contour


	public:

		Rect getBoundingRect();
		vector<Point> getPoints();
		float getArea();

		DataContour(vector<Point> contour);

		bool checkIfContourIsValid();

		static bool sortByBoundingRectXPosition(const DataContour& cwdLeft, const DataContour& cwdRight);     // this function allows us to sort                                                 // the contours from left to right
		

	
};

