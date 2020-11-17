

#include "opencv\cv.hpp"
#include "opencv2\ml.hpp"
#include "OCR.h"
#include "WordSearchPuzzle.h"
#include "Image.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

int main(int argc, char *argv[])
{
	// Read the image file
	Mat image = imread("images/WordSearch.jpg", CV_8UC1);
	// Check for failure
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	
	//Blur the image with 3x3 Gaussian kernel
	Mat image_blurred_with_3x3_kernel;
	GaussianBlur(image, image_blurred_with_3x3_kernel, Size(3, 3), 0);

	//Blur the image with 5x5 Gaussian kernel
	//Mat image_blurred_with_5x5_kernel;
	//GaussianBlur(image, image_blurred_with_5x5_kernel, Size(5, 5), 0);

	//Define names of the windows
	String window_name = "15 2";
	String window_name_blurred_with_3x3_kernel = "Lotus Blurred with 3 x 3 Gaussian Kernel";
	//String window_name_blurred_with_5x5_kernel = "Lotus Blurred with 5 x 5 Gaussian Kernel";

	// Create windows with above names
	//namedWindow(window_name);
	//namedWindow(window_name_blurred_with_3x3_kernel);
	//namedWindow(window_name_blurred_with_5x5_kernel);

	// Show our images inside the created windows.
	//imshow(window_name, image);
	//imshow(window_name_blurred_with_3x3_kernel, image_blurred_with_3x3_kernel);
	//imshow(window_name_blurred_with_5x5_kernel, image_blurred_with_5x5_kernel);

	//waitKey(0);

	//destroyAllWindows(); //destroy all opened windows

	Mat AT152;
	Mat AT132;
	Mat AT112;
	Mat AT92;
	Mat AT72;
	

	//BLOCK SIZE MÍNIMO 11 (de 11 a 15 va bien)
	//C MÍNIMO 3 (de 3 a 5 se comporta bien)
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT152, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 4);
	adaptiveThreshold(image_blurred_with_3x3_kernel,  AT132, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 3);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT112, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 2);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT92, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 1);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT72, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 15, 5);


	/*namedWindow(window_name);
	imshow(window_name, AT152);
	namedWindow("13 2");
	imshow("13 2", AT132);
	namedWindow("11 2");
	imshow("11 2", AT112);
	namedWindow("9 2");
	imshow("9 2", AT92);
	namedWindow("7 2");
	imshow("7 2", AT72);*/

	//Contorno

	Mat dst, cdst, contorno;

	Canny(AT72, dst, 50, 200, 3);
	imshow("edges", dst);
	waitKey(0);
	// Copy edges to the images that will display the results in BGR
	// Para qué sirve esto? Por qué no almacenar directamente, sirve para algo la clonación?
	cvtColor(dst, cdst, COLOR_GRAY2BGR);
	contorno = cdst.clone();

	imshow("contorno", contorno);
	waitKey(0);
	// Standard Hough Line Transform
	vector<Vec2f> lines; // will hold the results of the detection
	HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // runs the actual detection

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
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(dst, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection

	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		line(contorno, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255, 0, 0), 5, LINE_AA);
	}

	imshow("Original", image);
	imshow("Threshold_Origen", AT72); //Poner la imagen Threshold con mejor contorno
	imshow("Contorno", contorno);

	waitKey(0);
	destroyAllWindows();
	

	return 0;



}

