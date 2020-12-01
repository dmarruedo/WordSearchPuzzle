#include "WordSearchHelper.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>
#include<sstream>
#include<iostream>
#include<cstring>

vector<ImageContour> findWordSearhContours(Mat image, bool SHOW_IMAGE) {

	Mat processedImage = Mat(image.size(), CV_8UC1);
	resize(processedImage, processedImage, Size(processedImage.cols*2, processedImage.rows*2));

	//Codigo para aplicacion de filtro Gausiano  
	GaussianBlur(image, processedImage, Size(5, 5), 0);

	//Codigo para aplicacion de filtro binario
	adaptiveThreshold(processedImage, processedImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 11);

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


	if (SHOW_IMAGE)
	{
		namedWindow("Processed", CV_WINDOW_AUTOSIZE);
		imshow("Processed", processedImage);
		waitKey(0);
		destroyAllWindows();
	}


	//Se clona la imagen por si en algun momento se quiere usar processedImage para algo, ense�arla al final codigo o lo que sea
	Mat startImg = processedImage.clone();




	//almacena en el vector contours los vertices de cada uno de los contorno encontrados

	vector<vector<Point> > contours;
	findContours(startImg, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);



	//Se itera hasta encontrar en contorno de mayor area (p1) y el segundo de mayor area (p2), se filtra para que no busque en areas de menos de 25px (creo que son px)

	double area; double maxArea1 = 0; double maxArea2 = 0; int p1; int p2;

	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i], false);
		if (area > 25)
		{
			if (area > maxArea1)
			{
				maxArea2 = maxArea1;
				maxArea1 = area;
				p1 = i;
			}
			else if (area > maxArea2)
			{
				maxArea2 = area;
				p2 = i;
			}
		}
	}

	ImageContour extContour(contours[p1]);
	ImageContour cellContour(contours[p2]);

	if (SHOW_IMAGE)
	{
		Mat contourImage = processedImage.clone();
		cvtColor(contourImage, contourImage, COLOR_GRAY2BGR);

		drawContours(contourImage, contours, p1, Scalar(255, 0, 0), 1, 8);
		namedWindow("External Contour", CV_WINDOW_AUTOSIZE);
		imshow("External Contour", contourImage);
		waitKey(0);
		destroyAllWindows();

		contourImage = processedImage.clone();
		cvtColor(contourImage, contourImage, COLOR_GRAY2BGR);

		drawContours(contourImage, contours, p2, Scalar(255, 0, 0), 1, 8);
		namedWindow("Cell Contour", CV_WINDOW_AUTOSIZE);
		imshow("Cell Contour", contourImage);
		waitKey(0);
		destroyAllWindows();
	}

	vector< ImageContour> outContours;
	outContours.push_back(extContour);
	outContours.push_back(cellContour);

	return outContours;

}


int calculateRows(ImageContour wordSearchContour , ImageContour cellContour , double error) {

	int rows = wordSearchContour.getHeight() / (cellContour.getHeight() *(1 + error));
	return rows;

}

int calculateCols(ImageContour wordSearchContour, ImageContour cellContour, double error) {

	int cols = wordSearchContour.getWidth() / (cellContour.getWidth() *(1+error));
	return cols;

}


Mat extractCellImage(Mat wordSearchImage , int col, int row , double cellWidth , double cellHeight, bool SHOW_IMAGE) {
		
	Mat cellImage;
	cellImage = Mat(wordSearchImage, Rect((col-1)*cellWidth + 0.15*cellWidth, (row-1)*cellHeight + 0.15*cellHeight, cellWidth - 0.3*cellWidth, cellHeight - 0.3*cellHeight));
			// arrayCells[m][n] = cellImage;  Lo comento porque peta en ejecucion
	if (SHOW_IMAGE)
	{
		namedWindow("Celda", CV_WINDOW_AUTOSIZE);
		imshow("Celda", cellImage);
		waitKey(0);
		destroyAllWindows();
	}
	return cellImage;
}

Mat cutOnPerspective(Mat image, ImageContour contour , bool SHOW_IMAGE) {
	Point2f perpectivePoints[4];

	perpectivePoints[0] = Point2f(0, 0);		// top_left
	perpectivePoints[1] = Point2f(contour.getWidth(), 0);   // top_right
	perpectivePoints[2] = Point2f(contour.getWidth(), contour.getHeight());  //bottom_right
	perpectivePoints[3] = Point2f(0, contour.getHeight());   // bottom_left

	Point2f RealPoints[4];
	RealPoints[0] = contour.getPoints()[0];
	RealPoints[1] = contour.getPoints()[1];
	RealPoints[2] = contour.getPoints()[2];
	RealPoints[3] = contour.getPoints()[3];


	Mat wrap;
	Mat perpectiveImage;

	//Se reposiciona la imagen dentro del contorno en una imagen de resol x resol px
	perpectiveImage = Mat::zeros(image.size(), image.type());
	wrap = getPerspectiveTransform(RealPoints, perpectivePoints);
	warpPerspective(image, perpectiveImage, wrap, Size(contour.getWidth(), contour.getHeight()));

	if (SHOW_IMAGE)
	{
		namedWindow("perpectiveImage", CV_WINDOW_AUTOSIZE);
		imshow("perpectiveImage", perpectiveImage);
		waitKey(0);
		destroyAllWindows();
	}

	return perpectiveImage;
}



char OCR(String image , bool DEBUG_MODE) {
	vector<DataContour> allContoursWithData;           // declare empty vectors,
	vector<DataContour> validContoursWithData;         // we will fill these shortly



	Mat matClassificationInts;      // we will read the classification numbers into this variable as though it is a vector

	FileStorage fsClassifications("images/classifications.xml", FileStorage::READ);        // open the classifications file

	if (fsClassifications.isOpened() == false) {														// if the file was not opened successfully
		cout << "error, unable to open training classifications file, exiting program\n\n";				// show error message
		//return(0);																					// and exit program
	}

	fsClassifications["classifications"] >> matClassificationInts;      // read classifications section into Mat classifications variable
	fsClassifications.release();                                        // close the classifications file

																		// read in training images ////////////////////////////////////////////////////////////

	Mat matTrainingImagesAsFlattenedFloats;								// we will read multiple images into this single image variable as though it is a vector

	FileStorage fsTrainingImages("images/images.xml", FileStorage::READ);          // open the training images file

	if (fsTrainingImages.isOpened() == false) {                                                 // if the file was not opened successfully
		cout << "error, unable to open training images file, exiting program\n\n";				// show error message
	}

	fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;           // read images section into Mat training images variable
	fsTrainingImages.release();                                                 // close the traning images file

																				// train //////////////////////////////////////////////////////////////////////////////

	Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());				// instantiate the KNN object

																				// finally we get to the call to train, note that both parameters have to be of type Mat (a single Mat)
																				// even though in reality they are multiple images / numbers

	kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

	// TEST

	Mat matTestingNumbers = imread(image);            // read in the test numbers image
	resize(matTestingNumbers, matTestingNumbers, Size(matTestingNumbers.cols*2, matTestingNumbers.rows*2));

	if (matTestingNumbers.empty()) {                                // if unable to open image
		cout << "error: image not read from file\n\n";				// show error message on command line

	}

	Mat matGrayscale;           //
	Mat matBlurred;             // declare more image variables
	Mat matThresh;   
	Mat dilatedImage;//
	Mat matThreshCopy;          //

	cvtColor(matTestingNumbers, matGrayscale, CV_BGR2GRAY);

	GaussianBlur(matGrayscale, matBlurred, Size(3, 3), 0);
	if (DEBUG_MODE)
	{
		namedWindow("matBlurred", CV_WINDOW_AUTOSIZE);
		imshow("matBlurred", matBlurred);
		waitKey(0);
		destroyAllWindows();
	}

	adaptiveThreshold(matBlurred, matThresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 5, 7);

	Mat kernel;
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(matThresh, dilatedImage, kernel);

	matThreshCopy = dilatedImage.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

	if (DEBUG_MODE)
	{
		namedWindow("matThresh", CV_WINDOW_AUTOSIZE);
		imshow("matThresh", matThreshCopy);
		waitKey(0);
		destroyAllWindows();
	}

	vector<vector<Point> > ptContours;				// declare a vector for the contours
	vector<Vec4i> v4iHierarchy;                     // declare a vector for the hierarchy (we won't use this in this program but this may be helpful for reference)

	findContours(matThreshCopy, ptContours, v4iHierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < ptContours.size(); i++) {			 								// for each contour
		DataContour contourWithData(ptContours[i]);                                                    // instantiate a contour with data object

		allContoursWithData.push_back(contourWithData);                                     // add contour with data object to list of all contours with data
	}

	for (int i = 0; i < allContoursWithData.size(); i++) {                      // for all contours
		if (allContoursWithData[i].checkIfContourIsValid()) {                   // check if valid
			validContoursWithData.push_back(allContoursWithData[i]);            // if so, append to valid contour list
		}
	}
	//sort contours from left to right
	sort(validContoursWithData.begin(), validContoursWithData.end(), DataContour::sortByBoundingRectXPosition);

	string strFinalString;         // declare final string, this will have the final number sequence by the end of the program

	for (int i = 0; i < validContoursWithData.size(); i++) {            // for each contour

		rectangle(matTestingNumbers, validContoursWithData[i].getBoundingRect(), Scalar(0, 255, 0), 2);

		Mat matROI = matThresh(validContoursWithData[i].getBoundingRect());          // get ROI image of bounding rect

		Mat matROIResized;
		resize(matROI, matROIResized, Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));     // resize image, this will be more consistent for recognition and storage

		Mat matROIFloat;
		matROIResized.convertTo(matROIFloat, CV_32FC1);             // convert Mat to float, necessary for call to find_nearest

		Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);

		Mat matCurrentChar(0, 0, CV_32F);

		kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);     // finally we can call find_nearest !!!

		float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);

		strFinalString = strFinalString + char(int(fltCurrentChar));        // append current char to full string

	}


	if (DEBUG_MODE)
	{
		cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";       // show the full string

		imshow("matTestingNumbers", matTestingNumbers);     // show input image with green boxes drawn around found digits

		waitKey(0);
		// wait for user key press
	}

	return strFinalString[0];
}

void WordSearch(vector<vector<char>>sopaLetras, int columns, int rows)
{


	bool wordFound = false;
	struct WordPos values;

	cout << "\n" << "Ingrese la palabra que est� buscando en la sopa de letras" << "\n";
	string palabra;
	cin >> palabra;
	cout << "\n" << "Est� buscando la palabra: " << palabra << "\n";

	int wordLength;
	wordLength = palabra.size();

	char caracteres(wordLength - 1);
	int pos = 0;

	string checkWord;

	for (int i = 0; i <= columns; i++)
	{
		for (int j = 0; j <= rows; j++)
		{
			//encontramos la primera letra de la palabra que estamos buscando
			//if (strcmp(palabra[pos],sopaLetras[i][j])==0)
			if (palabra[pos] == sopaLetras[i][j])
			{
				//comprobamos si la palabra entra en la direcci�n estimada
				//de izquierda a derecha
				if (wordLength < columns - i)
				{
					//si la palabra entra en la matriz, comprobamos 
					//primero creamos string con los caracteres de esa direcci�n y para ese tama�o
					for (int a = 1; a < wordLength; a++)
					{
						checkWord.push_back(sopaLetras[i + a][j]);
					}
					//comprobaci�n de la palabra, si es correcto, almacenamos posici�n, direcci�n y tama�o

					if (checkWord.compare(palabra) == 0)
					{
						values.Xpos = i;
						values.Ypos = j;
						values.Xdir = 1;
						values.Ydir = 0;
						values.wordSize = wordLength;
						break;
						wordFound = true;
					}
				}
				else {
					cout << "\n" << "No entra en esta direccion" << "\n";
				}
				//comprobamos de derecha a izquierda
				if (wordLength <= i + 1)
				{
					for (int a = 1; a < wordLength; a++)
					{
						checkWord.push_back(sopaLetras[i - a][j]);
					}
					//comprobaci�n de la palabra, si es correcto, almacenamos posici�n, direcci�n y tama�o

					if (checkWord.compare(palabra) == 0)
					{
						values.Xpos = i;
						values.Ypos = j;
						values.Xdir = -1;
						values.Ydir = 0;
						values.wordSize = wordLength;
						break;
						wordFound = true;
					}
				}
				else {
					cout << "\n" << "No entra en esta direccion" << "\n";
				}
				//de arriba a abajo
				if (wordLength < rows - j)
				{
					//si la palabra entra en la matriz, comprobamos 
					//primero creamos string con los caracteres de esa direcci�n y para ese tama�o
					for (int a = 1; a < wordLength; a++)
					{
						checkWord.push_back(sopaLetras[i][j + a]);
					}
					//comprobaci�n de la palabra, si es correcto, almacenamos posici�n, direcci�n y tama�o

					if (checkWord.compare(palabra) == 0)
					{
						values.Xpos = i;
						values.Ypos = j;
						values.Xdir = 0;
						values.Ydir = 1;
						values.wordSize = wordLength;
						break;
						wordFound = true;
					}
				}
				else {
					cout << "\n" << "No entra en esta direccion" << "\n";
				}
				//comprobamos de abajo a arriba
				if (wordLength <= j + 1)
				{
					for (int a = 1; a < wordLength; a++)
					{
						checkWord.push_back(sopaLetras[i][j - a]);
					}
					//comprobaci�n de la palabra, si es correcto, almacenamos posici�n, direcci�n y tama�o

					if (checkWord.compare(palabra) == 0)
					{
						values.Xpos = i;
						values.Ypos = j;
						values.Xdir = 0;
						values.Ydir = -1;
						values.wordSize = wordLength;
						break;
						wordFound = true;
					}
				}
				else {
					cout << "\n" << "No entra en esta direccion" << "\n";
				}
			}
		}
		//para salir del segundo for
		if (wordFound)
		{
			break;
		}

	}

	cout << "la palabra buscada es: " << palabra << "\n";
	cout << "la palabra encontrada es: " << checkWord << "\n";
	cout << "la primera letra se encuentra en: " << values.Xpos << values.Ypos << "\n";
	cout << "con direcci�n: " << values.Xdir << values.Ydir << "\n";
	cout << "y tama�o: " << values.wordSize << "\n";

	return;
};
