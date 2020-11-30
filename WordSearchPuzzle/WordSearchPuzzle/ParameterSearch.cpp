

#include "opencv\cv.hpp"
#include "opencv2\ml.hpp"
#include "WordSearchPuzzle.h"


using namespace std;
using namespace cv;
using namespace cv::ml;

int ParameterSearch(void)
{
	// Read the image file
	Mat image = imread("images/sopafoto2.jpg", CV_8UC1);
	Mat image1 = imread("images/sopafoto2.jpg", CV_8UC1);
	Mat image2 = imread("images/sopafoto2.jpg", CV_8UC1);
	// Check for failure
	if (image.empty() || image1.empty() || image2.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}


	//Blur the image with 3x3 Gaussian kernel
	Mat imageB;
	Mat imageB1;
	Mat imageB2;
	GaussianBlur(image, imageB, Size(3, 3), 0);
	GaussianBlur(image, imageB1, Size(3, 3), 0);
	GaussianBlur(image, imageB2, Size(3, 3), 0);

	//Blur the image with 5x5 Gaussian kernel
	//Mat image_blurred_with_5x5_kernel;
	//GaussianBlur(image, image_blurred_with_5x5_kernel, Size(5, 5), 0);

	//Define names of the windows
	String window_name = "15 2";
	String window_name_blurred_with_3x3_kernel = "Lotus Blurred with 3 x 3 Gaussian Kernel";
	//String window_name_blurred_with_5x5_kernel = "Lotus Blurred with 5 x 5 Gaussian Kernel";



	Mat imageAT;
	Mat imageAT1;
	Mat imageAT2;

	vector <Mat> imagevector;
	vector <Mat> image1vector;
	vector <Mat> image2vector;
	vector <Mat> gridvector;
	vector <Mat> grid1vector;
	vector <Mat> grid2vector;
	//BLOCK SIZE MÍNIMO 11 (de 11 a 15 va bien)
	//C MÍNIMO 3 (de 3 a 5 se comporta bien)
	int j;
	int count = 3;
	int bs = 10;
	int c = 3;
	for (j = 1; j < count; j++) {

		adaptiveThreshold(imageB, imageAT, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, bs + j, c);
		imagevector.push_back(imageAT);

		adaptiveThreshold(imageB, imageAT1, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, bs + j, c);
		image1vector.push_back(imageAT1);

		adaptiveThreshold(imageB, imageAT2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, bs + j, c);
		image2vector.push_back(imageAT2);

		Mat dst, dst1, dst2;
		Mat cdst, cdst1, cdst2;
		Mat contorno, contorno1, contorno2;

		Canny(imageAT, dst, 50, 200, 3);
		Canny(imageAT1, dst1, 50, 200, 3);
		Canny(imageAT2, dst2, 50, 200, 3);


		cvtColor(dst, cdst, COLOR_GRAY2BGR);
		cvtColor(dst1, cdst1, COLOR_GRAY2BGR);
		cvtColor(dst2, cdst2, COLOR_GRAY2BGR);

		contorno = cdst.clone();
		contorno1 = cdst1.clone();
		contorno2 = cdst2.clone();

		// Standard Hough Line Transform
		vector<Vec2f> lines; // will hold the results of the detection
		vector<Vec2f> lines1;
		vector<Vec2f> lines2;
		HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
		HoughLines(dst1, lines1, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
		HoughLines(dst2, lines2, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection
		// Draw the lines
		for (size_t i = 0; i < lines.size(); i++)
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		}
		for (size_t i = 0; i < lines1.size(); i++)
		{
			float rho = lines1[i][0], theta = lines1[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst1, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		}
		for (size_t i = 0; i < lines2.size(); i++)
		{
			float rho = lines2[i][0], theta = lines2[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a * rho, y0 = b * rho;
			pt1.x = cvRound(x0 + 1000 * (-b));
			pt1.y = cvRound(y0 + 1000 * (a));
			pt2.x = cvRound(x0 - 1000 * (-b));
			pt2.y = cvRound(y0 - 1000 * (a));
			line(cdst2, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
		}
		// Probabilistic Line Transform
		vector<Vec4i> linesP; // will hold the results of the detection
		vector<Vec4i> linesP1;
		vector<Vec4i> linesP2;
		HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
		HoughLinesP(dst1, linesP1, 1, CV_PI / 180, 50, 50, 10);
		HoughLinesP(dst2, linesP2, 1, CV_PI / 180, 50, 50, 10);
		// Draw the lines
		for (size_t i = 0; i < linesP.size(); i++)
		{
			Vec4i l = linesP[i];
			line(contorno, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 5, LINE_AA);
		}
		for (size_t i = 0; i < linesP1.size(); i++)
		{
			Vec4i l = linesP1[i];
			line(contorno1, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 5, LINE_AA);
		}
		for (size_t i = 0; i < linesP2.size(); i++)
		{
			Vec4i l = linesP2[i];
			line(contorno2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 5, LINE_AA);
		}

		imshow("Contorno", contorno);
		gridvector.push_back(contorno);
		imshow("Contorno 1", contorno1);
		grid1vector.push_back(contorno);
		imshow("Contorno 2", contorno2);
		grid2vector.push_back(contorno2);
		waitKey(0);
		destroyAllWindows();
	}

		imshow("it1", gridvector[0]);
		imshow("it2", gridvector[1]);
		imshow("it3", gridvector[2]);
		imshow("it4", gridvector[3]);
		imshow("it5", gridvector[4]);
		waitKey(0);
		destroyAllWindows();
		imshow("it1", grid1vector[0]);
		imshow("it2", grid1vector[1]);
		imshow("it3", grid1vector[2]);
		imshow("it4", grid1vector[3]);
		imshow("it5", grid1vector[4]);
		waitKey(0);
		destroyAllWindows();
		imshow("it1", grid2vector[0]);
		imshow("it2", grid2vector[1]);
		imshow("it3", grid2vector[2]);
		imshow("it4", grid2vector[3]);
		imshow("it5", grid2vector[4]);
		waitKey(0);
		destroyAllWindows();
	return 0;

}

