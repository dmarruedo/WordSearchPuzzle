

#include "opencv\cv.hpp"
#include "opencv2\ml.hpp"//m
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
	/*Mat image_blurred_with_5x5_kernel;
	GaussianBlur(image, image_blurred_with_5x5_kernel, Size(5, 5), 0);*/

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
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT152, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 4);
	adaptiveThreshold(image_blurred_with_3x3_kernel,  AT132, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 3);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT112, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 2);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT92, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 1);
	adaptiveThreshold(image_blurred_with_3x3_kernel, AT72, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 5);

	namedWindow(window_name);
	imshow(window_name, AT152);
	namedWindow("13 2");
	imshow("13 2", AT132);
	namedWindow("11 2");
	imshow("11 2", AT112);
	namedWindow("9 2");
	imshow("9 2", AT92);
	namedWindow("7 2");
	imshow("7 2", AT72);



	waitKey(0);
	destroyAllWindows();


	return 0;
}

