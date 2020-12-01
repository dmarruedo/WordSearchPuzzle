// SopaLetasPrueba.cpp : Este archivo contiene la funci�n "main". La ejecuci�n del programa comienza y termina ah�.
//

#include <iostream>
#include <ctime> 
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


	unsigned t0, t1;
	t0 = clock();

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

	Mat perspectiveImage = cutOnPerspective(resizeImage, extContour, DEBUG_MODE);
	vector<vector<char>> wordSearchMapChars;
	vector<char> rowChars;

	Ptr<cv::ml::KNearest> KNearest= loadOCR();


	for (int m = 1; m <= rows; m++) {
		for (int n = 1; n <= cols; n++) {
			Mat cell = extractCellImage(perspectiveImage, n, m, cellWidth, cellHeight, DEBUG_MODE);
			imwrite("Celdas/Celda.jpg", cell);
			char charOCR = OCR("Celdas/Celda.jpg", KNearest, DEBUG_MODE);
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

	vector<vector<Point>> solution = wordSearchMap.getSolution();
	Mat roundsImage = perspectiveImage.clone();

	//bucle que va redondeando/pintando las palabras encontradas en la imagen de la sopa de letras , solution es un vector con las posiciones x(fila) e y(columna) de los caracateres encontrados para cada palabra
	for (int i = 0;i< solution.size(); i++) {
	//for (int i = 0; i < 1; i++) {

		//la funcion roundWord esta sin codigo al final del archivo WordSearchHelper.cpp
		roundsImage = roundWord(roundsImage, solution[i], rows, cols, cellWidth, cellHeight, DEBUG_MODE);
	
	}
	

	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	cout << "\n\nTiempo de ejecucion: " << time << endl;

	//Muestra la sopa de letras resuelta
	namedWindow("Solucion", CV_WINDOW_AUTOSIZE);
	imshow("Solucion", roundsImage);
	waitKey(0);
	destroyAllWindows();



	return 1;
}



