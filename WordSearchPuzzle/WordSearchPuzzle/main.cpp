// SopaLetasPrueba.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <ctime> 
#include "WordSearchHelper.h"
#include "WordSearchPuzzle.h"




using namespace cv;
using namespace std;


const bool DEBUG_MODE = true;


//const String IMGDIR = "images/WordSearchPuzzle1.jpg";
const String IMGDIR = "images/WordSearchPuzzle2.jpg";

int main(int argc, char *argv[])
{


	unsigned t0, t1;
	t0 = clock();

	Mat originalImage = imread(IMGDIR, IMREAD_COLOR);

	if (originalImage.empty()) {                                
		cout << "error: no se puede abrir la imagen : " << IMGDIR << "\n\n";
		return 1;// 

	}


	cout << "Imagen " << IMGDIR << " abierta \n\n";

	vector<String> words;

	if (IMGDIR == "images/WordSearchPuzzle1.jpg")
	{
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
	}
	else if (IMGDIR == "images/WordSearchPuzzle2.jpg")
	{
		words.push_back("VISA");
		words.push_back("ANIMACION");
		words.push_back("AVIACION");
		words.push_back("VACACIONES");
		words.push_back("RECREACION");
		words.push_back("VISITANTE");
		words.push_back("CULTURA");
		words.push_back("SUSTENTABLE");
		words.push_back("VIAJERO");
		words.push_back("PATRIMONIO");
		words.push_back("EXCURSION");
		words.push_back("PASAPORTE");
		words.push_back("HOSTELERIA");
		words.push_back("ECOTURISMO");
		words.push_back("CONGRESO");
	}
	
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

	cout << "Contorno detectados \n\n";
	
	wordSearchContours.erase(wordSearchContours.begin());
	vector<ImageContour> cellContours = wordSearchContours;

	vector<vector<ImageContour>> cellContourMatrix = orderCellContour(extContour, cellContours);

	int rows = cellContourMatrix.size();
	int cols = cellContourMatrix[0].size();

	cout << "Se detectan: " << rows << " filas y " << cols << " columnas \n\n";

	vector<vector<char>> wordSearchMapChars;
	vector<char> rowChars;


	Ptr<cv::ml::KNearest> KNearest= loadOCR();

	bool debug = false;

	for (int m = 0; m < rows; m++) {
		for (int n = 0; n < cols; n++) {
			if (DEBUG_MODE && m==0 && n < 2) {
				debug = true;
			}
			else {
				debug = false;
			}			
			Mat cell = extractCellImage(resizeImage, cellContourMatrix[m][n], debug);
			char charOCR = OCR(cell, KNearest, debug);
			rowChars.push_back(charOCR);
		}
		wordSearchMapChars.push_back(rowChars);
		rowChars.clear();
	}




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
	
	Mat perspectiveImage = cutOnPerspective(resizeImage, extContour, false);
	namedWindow("Solucion", CV_WINDOW_AUTOSIZE);
	imshow("Solucion", perspectiveImage);
	waitKey(0);
	destroyAllWindows();

	//Guarda la imagen con la sopa de letras resuelta
	imwrite("images/WordSearchPuzzleSolucion.jpg", roundsImage);

	cout << "Solcuion guardada en images/WordSearchPuzzleSolucion.jpg";


	return 1;
}



