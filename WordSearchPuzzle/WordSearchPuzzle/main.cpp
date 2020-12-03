// SopaLetasPrueba.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <ctime> 
#include "WordSearchHelper.h"
#include "WordSearchPuzzle.h"




using namespace cv;
using namespace std;


const bool DEBUG_MODE = false;


const String IMGDIR = "images/sopafoto2.jpg";
//const String IMGDIR = "images/WordSearch1.jpg";

int main(int argc, char *argv[])
{


	unsigned t0, t1;
	t0 = clock();

	Mat originalImage = imread(IMGDIR, IMREAD_COLOR);

	cout << "Imagen " << IMGDIR << " abierta \n\n";
	
	Mat resizeImage;

	double scaleY = 1000 / (double) originalImage.rows;
	double scaleX = 1000 / (double) originalImage.rows;

	double scale = min(scaleY, scaleX);

	
	resize(originalImage, resizeImage, Size(originalImage.cols*scale, originalImage.rows*scale));
	


	if (DEBUG_MODE)
	{
		namedWindow("Original", CV_WINDOW_AUTOSIZE);
		imshow("Original", resizeImage);
		waitKey(0);
		destroyAllWindows();
	}

	vector<ImageContour> wordSearchContours = findWordSearhContours(resizeImage, DEBUG_MODE);
	ImageContour extContour = wordSearchContours[0];

	cout << "Contorno detectados";
	
	wordSearchContours.erase(wordSearchContours.begin());
	vector<ImageContour> cellContours = wordSearchContours;

	vector<vector<ImageContour>> cellContourMatrix = orderCellContour(extContour, cellContours);

	int rows = cellContourMatrix.size();
	int cols = cellContourMatrix[0].size();

	cout << "Se detectan: " << rows << " filas y " << cols << " columnas \n\n";

	vector<vector<char>> wordSearchMapChars;
	vector<char> rowChars;


	Ptr<cv::ml::KNearest> KNearest= loadOCR();

	

	for (int m = 0; m < rows; m++) {
		for (int n = 0; n < cols; n++) {
			Mat cell = extractCellImage(resizeImage, cellContourMatrix[m][n],false);
			char charOCR = OCR(cell, KNearest, false);
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

	Mat roundsImage;

	//bucle que va redondeando/pintando las palabras encontradas en la imagen de la sopa de letras , solution es un vector con las posiciones x(fila) e y(columna) de los caracateres encontrados para cada palabra
	for (int i = 0;i< solution.size(); i++) {

		roundsImage = roundWord(resizeImage, cellContourMatrix, solution[i], DEBUG_MODE);
	}
	

	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	cout << "\n\nTiempo de ejecucion: " << time << endl;

	
	//Muestra la sopa de letras resuelta
	
	Mat perspectiveImage = cutOnPerspective(resizeImage, extContour, DEBUG_MODE);
	namedWindow("Solucion", CV_WINDOW_AUTOSIZE);
	imshow("Solucion", perspectiveImage);
	waitKey(0);
	destroyAllWindows();

	//Guarda la imagen con la sopa de letras resuelta
	imwrite("images/WordSearchPuzzleSolucion.jpg", roundsImage);


	return 1;
}



