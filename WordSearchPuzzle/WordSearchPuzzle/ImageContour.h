#pragma once

#include <iostream>
#include "opencv\cv.hpp"
#include "ImageContour.h"



using namespace cv;
using namespace std;

class ImageContour
{

private:
	vector<Point> points;
	double height;
	double width;
                             // area of contour

	

	int calculateWidth(vector<Point> contour);
	int calculateHeight(vector<Point> contour);

	vector<Point> clockwiseOrderVertix(vector<Point> contour);



public:
	ImageContour();
	ImageContour(vector<Point> contour);
	vector<Point> getPoints();
	double getWidth();
	double getHeight();
                           // obviously in a production grade progra                                         // identifying if a contour is valid !!
	

	/*
	static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
	}
	*/



};

