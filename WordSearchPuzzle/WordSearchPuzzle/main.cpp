// SopaLetasPrueba.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "WordSearchHelper.h"
#include "WordSearchPuzzle.h"
#include "WordSearch.cpp"



using namespace cv;
using namespace std;



const bool DEBUG_MODE = false;
//const double SCALE = 0.25;
const double LINE_WIDTH = 0.07;


const String IMGDIR = "images/sopafoto2.jpg";
//const String IMGDIR = "images/WordSearch.jpg";

int main(int argc, char *argv[])
{

	double SCALE = 0.25;
	Mat originalImage = imread(IMGDIR, 0);
	Mat resizeImage;

	if (originalImage.size().width < 1000) {
		SCALE = 1;
	}
	resize(originalImage, resizeImage, Size(originalImage.cols*SCALE, originalImage.rows*SCALE));

	if (DEBUG_MODE)
	{
		namedWindow("Original", CV_WINDOW_AUTOSIZE);
		imshow("Original", originalImage);
		waitKey(0);
		destroyAllWindows();
	}


	if (DEBUG_MODE)
	{
		namedWindow("Resized", CV_WINDOW_AUTOSIZE);
		imshow("Resized", resizeImage);
		waitKey(0);
		destroyAllWindows();
	}

	vector<ImageContour> wordSearchContours = findWordSearhContours(resizeImage, DEBUG_MODE);
	ImageContour extContour = wordSearchContours[0];
	ImageContour cellContour = wordSearchContours[1];

	int rows = calculateRows(extContour, cellContour, LINE_WIDTH);
	int cols = calculateCols(extContour, cellContour, LINE_WIDTH);
	double cellWidth = extContour.getWidth() / cols;
	double cellHeight = extContour.getHeight() / rows;

	Mat perpectiveImage = cutOnPerspective(resizeImage, extContour, DEBUG_MODE);
	vector<vector<char>> wordSearchMapChars;
	vector<char> rowChars;

	for (int m = 1; m <= rows; m++) {
		for (int n = 1; n <= cols; n++) {

			Mat cell = extractCellImage(perpectiveImage, n, m, cellWidth, cellHeight, false);
			imwrite("Celdas/Celda.jpg", cell);
			char charOCR = OCR("Celdas/Celda.jpg", false);
			rowChars.push_back(charOCR);
		}
		wordSearchMapChars.push_back(rowChars);
		rowChars.clear();
	}


	vector<String> words;
	words.push_back("CELDA");
	words.push_back("NUMERO");

	//struct WordPos sol;
	//WordSearchPuzzle wordSearchMap(wordSearchMapChars, words);

	WordSearch(wordSearchMapChars, cols, rows);

	//wordSearchMap.printConsole();

	//wordSearchMap.solve();


	return 1;
}



