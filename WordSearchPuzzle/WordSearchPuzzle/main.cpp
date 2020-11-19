// SopaLetasPrueba.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "opencv\cv.hpp"


using namespace cv;
using namespace std;


// Esta funcion esta en el codigo, no consigo que en el argumento vector vertices llegue con mas de una posicion... y no lo entiendo ahi lo dejo 
// Es para calcular la altura y anchura de los contornos
void calculateDimensions(double heigth, double width, Point2f vertices[])
{
	double heigth1; double heigth2; double width1; double width2;
	width1 = vertices[1].x - vertices[0].x;
	width2 = vertices[2].x - vertices[3].x;
	width = (width1 + width2) / 2;

	heigth1 = vertices[3].y - vertices[0].y;
	heigth2 = vertices[2].y - vertices[1].y;
	heigth = (heigth1 + heigth2) / 2;

}


// la funcion findcontours tal como la tenemos configuradas devuelve la variable contours con los vertices de los contornos encontrados, el problema es que no vienen ordenados 
// Esta funcion devuelve un vector de Point (x,y) ordenados Top_Right, Top_Left , Bottom_Left , Bottom_right

void clockwiseOrderVertix(int p, vector<vector<Point> > contours, Point2f* vertices)
{
	double sum;
	double prevsum;

	int top_right; int bottom_left; int bottom_right; int top_left;
	// codigo para asignar los vertices encontrados del cuadrado exterior a su posicion en el plano (top-Left , top_right , bottom_right , bottom_left)
	prevsum = contours[p][0].x + contours[p][0].y;
	bottom_right = 0;
	for (int i = 0; i < 4; i++)
	{
		sum = contours[p][i].x + contours[p][i].y;
		if (sum > prevsum)
		{
			prevsum = sum;
			bottom_right = i;
		}
	}
	prevsum = contours[p][0].x + contours[p][0].y;
	top_left = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contours[p][i].x + contours[p][i].y;
		if (sum <= prevsum)
		{
			prevsum = sum;
			top_left = i;
		}
	}
	prevsum = contours[p][0].x - contours[p][0].y;
	bottom_left = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contours[p][i].x - contours[p][i].y;
		if (sum <= prevsum)
		{
			prevsum = sum;
			bottom_left = i;
		}
	}

	prevsum = contours[p][0].x - contours[p][0].y;
	top_right = 0;
	for (int i = 1; i < 4; i++)
	{
		sum = contours[p][i].x - contours[p][i].y;
		if (sum > prevsum && i != top_left)
		{
			prevsum = sum;
			top_right = i;
		}
	}

	vertices[0] = contours[p][top_left];
	vertices[1] = contours[p][top_right];
	vertices[2] = contours[p][bottom_right];
	vertices[3] = contours[p][bottom_left];

}


const bool SHOWIMAGE = true;
const String IMGDIR = "images/WordSearch.jpg";
//#define const String imgDir = "images/WordSearch1.jpg";

int main(int argc, char *argv[])
{

	Mat OriginalImage = imread(IMGDIR, 0);

	if (SHOWIMAGE)
	{
		namedWindow("Original", CV_WINDOW_AUTOSIZE);
		imshow("Original", OriginalImage);
		waitKey(0);
		destroyAllWindows();
	}

	// Se crea una imagen nueva vacia a partir de las propiedas de la original
	Mat processedImage = Mat(OriginalImage.size(), CV_8UC1);

	//Codigo para aplicacion de filtro Gausiano  
	GaussianBlur(OriginalImage, processedImage, Size(5, 5), 0);

	//Codigo para aplicacion de filtro binario
	adaptiveThreshold(processedImage, processedImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 5, 10);

	//Codigo para invertir colores binarios 
	bitwise_not(processedImage, processedImage);

	// Codigo para dilatar o erosionar la imagen binaria, no parece necesario de momento pero dejo el codigo por si acaso
	/*
	Mat kernel
	kernel = getStructuringElement(MORPH_CROSS, Size(1, 1));
	dilate(processedImage, processedImage, kernel);

	kernel = getStructuringElement(MORPH_CROSS, Size(1, 1));
	erode(processedImage, processedImage, kernel);
	*/


	if (SHOWIMAGE)
	{
		namedWindow("Processed", CV_WINDOW_AUTOSIZE);
		imshow("Processed", processedImage);
		waitKey(0);
		destroyAllWindows();
	}

	//Se clona la imagen por si en algun momento se quiere usar processedImage para algo, enseñarla al final codigo o lo que sea
	Mat startImg = processedImage.clone();




	//almacena en el vector contours los vertices de cada uno de los contorno encontrados

	vector<vector<Point> > contours;
	findContours(startImg, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);




	//Se itera hasta encontrar en contorno de mayor area (p1) y el segundo de mayor area (p2), se filtra para que no busque en areas de menos de 25px (creo que son px)

	double area; double maxarea1 = 0; double maxarea2 = 0; int p1; int p2;

	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i], false);
		if (area > 25)
		{
			if (area > maxarea1)
			{
				maxarea2 = maxarea1;
				maxarea1 = area;
				p1 = i;
			}
			else if (area > maxarea2)
			{
				maxarea2 = area;
				p2 = i;
			}
		}
	}



	// Puede ser que el contorno tenga curvas o no sea rectangular, el codigo siguiente elimina estras imperfecciones

	double perimeter;

	// Se aproxima el contorno a un poligono de cuatro vertices para el contorno exterior
	perimeter = arcLength(contours[p1], true);
	approxPolyDP(contours[p1], contours[p1], 0.01*perimeter, true);

	// Se Aproxima el contorno a un poligono de cuatro vertices para el contorno de la celda 
	perimeter = arcLength(contours[p2], true);
	approxPolyDP(contours[p2], contours[p2], 0.10*perimeter, true);


	// Se pintan en startImg los dos contornos de mayor area encontrados , en teoria el cuadrado exterior y el cuadrado de una de las celdas , primero se pone a color la imagen, el treshold la pasa a formato binario (negro, blanco)
	cvtColor(startImg, startImg, COLOR_GRAY2BGR);

	drawContours(startImg, contours, p1, Scalar(255, 0, 0), 1, 8);
	drawContours(startImg, contours, p2, Scalar(255, 0, 0), 1, 8);


	if (SHOWIMAGE)
	{
		namedWindow("contornos", CV_WINDOW_AUTOSIZE);
		imshow("contornos", startImg);
		waitKey(0);
		destroyAllWindows();
	}


	Point2f outSquarePoints[4];

	// Funcion definida debajo de main para ordenar los extremos del contorno de la sopa de letras en sentido horario , es necesario para calcular la altura y ancho y tambien para adaptar la perpectiva (si no podria quedar girada o desconfirgurada)
	// outSquarePoints es el vector con las esquinas ordenadas
	clockwiseOrderVertix(p1, contours, outSquarePoints);

	Point2f inSquarePoints[4];

	// Funcion definida debajo de main para ordenar los extremos del contorno de las celdas en sentido horario, solo necesario ordenarlas
	// intSquarePoints es el vector con las esquinas ordenadas
	clockwiseOrderVertix(p2, contours, inSquarePoints);

	double heightOutSquare = 0; double widthOutSquare = 0;
	//calculateDimensions(heightOutSquare,  widthOutSquare, outSquarePoints);

	double heightInSquare = 0; double widthInSquare = 0;
	//calculateDimensions( heightInSquare, widthInSquare, inSquarePoints);


	//Esta parte del codigo es para calcular la altura y ancho de los contornos, la funcion CalculateDimension solo coge la primera posicion de outSquarePoints y me cago en todo por que no se que cojones le pasa
	double heigth1; double heigth2; double width1; double width2;
	width1 = outSquarePoints[1].x - outSquarePoints[0].x;
	width2 = outSquarePoints[2].x - outSquarePoints[3].x;
	widthOutSquare = (width1 + width2) / 2;

	heigth1 = outSquarePoints[3].y - outSquarePoints[0].y;
	heigth2 = outSquarePoints[2].y - outSquarePoints[1].y;
	heightOutSquare = (heigth1 + heigth2) / 2;


	width1 = inSquarePoints[1].x - inSquarePoints[0].x;
	width2 = inSquarePoints[2].x - inSquarePoints[3].x;
	widthInSquare = (width1 + width2) / 2;

	heigth1 = inSquarePoints[3].y - inSquarePoints[0].y;
	heigth2 = inSquarePoints[2].y - inSquarePoints[1].y;
	heightInSquare = (heigth1 + heigth2) / 2;



	// A partir de la altura y ancho calculamos el numero de filas y columnas, se define una correccion por el grosor de las lineas (definido ahora en 5%)

	double correccionLine = 0.05;
	int rows = heightOutSquare / (heightInSquare * (1 + correccionLine));
	int cols = widthOutSquare / (widthInSquare *(1 + correccionLine));


	//se define una resolucion multiplo de las filas y columnas para que cuando estraigamos la celdas no se corten, (no pueden ser la celda de 46,87 pixel, solo numero enteros creo)

	int resol = rows * cols * 5;


	// Se define las posiciones de la imagen de los corners para luego reposicionarla en perpectiva
	Point2f perpectivePoints[4];

	perpectivePoints[0] = Point2f(0, 0);		// top_left
	perpectivePoints[1] = Point2f(resol, 0);   // top_right
	perpectivePoints[2] = Point2f(resol, resol);  //bottom_right
	perpectivePoints[3] = Point2f(0, resol);   // bottom_left

	Mat wrap;
	Mat perpectiveImage;

	//Se reposiciona la imagen dentro del contorno en una imagen de resol x resol px
	perpectiveImage = Mat::zeros(OriginalImage.size(), OriginalImage.type());
	wrap = getPerspectiveTransform(outSquarePoints, perpectivePoints);
	warpPerspective(OriginalImage, perpectiveImage, wrap, Size(resol, resol));

	if (SHOWIMAGE)
	{
		namedWindow("perpectiva", CV_WINDOW_AUTOSIZE);
		imshow("perspectiva", perpectiveImage);
		waitKey(0);
		destroyAllWindows();
	}


	//codigo para extrar las celdas de la imagen en perpectiva correcta, creo que los for estan bien pero al ser la imagen cuadra no lo se, pueden que estan confundidas filas y columnas
	//El orden de salida es fila a fila de izquierda a derecha y de arriba a abajo.
	double cellWidth = resol / cols;
	double celdlength = resol / rows;
	Mat cellImage;
	vector<vector<Mat>> arrayCells;
	int m = 0; int n;
	for (m = 0; m < resol; m = m + cellWidth)
	{
		for (n = 0; n < resol; n = n + celdlength)
		{
			cellImage = Mat(perpectiveImage, Rect(n, m, cellWidth, celdlength));
			// arrayCells[m][n] = cellImage;  Lo comento porque peta en ejecucion

			if (SHOWIMAGE)
			{
				namedWindow("Celda", CV_WINDOW_AUTOSIZE);
				imshow("Celda", cellImage);
				waitKey(0);
				destroyAllWindows();
			}

		}
	}

	return 1;
}


