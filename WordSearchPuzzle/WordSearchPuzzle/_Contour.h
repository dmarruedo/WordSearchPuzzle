#pragma once

#include <iostream>
#include "opencv\cv.hpp"

using namespace cv;
using namespace std;

class _Contour{
	private:

		vector<Point2f> contour;
		int height;
		int width;
		int	resolution;

	public:
		_Contour(vector<Point2f>);
		void setHeight(int heigth);
		void setWidth(int width);
		void setResolution(int resolution);
		int getHeigth();
		int getWidth();
		int getResolution();
		vector<Point2f> getContour();
		

};

