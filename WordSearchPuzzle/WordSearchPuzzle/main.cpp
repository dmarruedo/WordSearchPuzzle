// SopaLetasPrueba.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "WordSearchHelper.h"
#include "WordSearchPuzzle.h"




using namespace cv;
using namespace std;


const bool DEBUG_MODE = false;
const double SCALE = 0.25;
const double LINE_WIDTH = 0.07;


const String IMGDIR = "images/sopafoto2.jpg";
//const String IMGDIR = "images/WordSearch1.jpg";

int main(int argc, char *argv[])
{

	Mat originalImage = imread(IMGDIR, 0);
	Mat resizeImage;
	resize(originalImage, resizeImage, Size(originalImage.cols*SCALE, originalImage.rows*SCALE));



	if (DEBUG_MODE)
	{
		namedWindow("Original", CV_WINDOW_AUTOSIZE);
		imshow("Original", resizeImage);
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
			Mat cell = extractCellImage(perpectiveImage, n, m, cellWidth, cellHeight, DEBUG_MODE);
			imwrite("Celdas/Celda.jpg", cell);
			char charOCR = OCR("Celdas/Celda.jpg", DEBUG_MODE);
			rowChars.push_back(charOCR);
		}
		wordSearchMapChars.push_back(rowChars);
		rowChars.clear();
	}


	vector<String> words;
	words.push_back("CELDA");
	words.push_back("NUMERO");
	words.push_back("LIBRO");
	words.push_back("RENGLON");
	words.push_back("FUNCIONES");
	words.push_back("FORMULA");
	words.push_back("EXCEL");
	words.push_back("GRAFICO");
	words.push_back("MONEDA");
	words.push_back("COLUMNA");

	WordSearchPuzzle wordSearchMap(wordSearchMapChars, words);

	wordSearchMap.printConsole();

	wordSearchMap.solve();


	return 1;
}



