#include "WordSearchHelper.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>
#include<sstream>
#define PI 3.14159265
#define RAND_MAX 255
vector<ImageContour> findWordSearhContours(Mat image, bool SHOW_IMAGE) {

	Mat processedImage = Mat(image.size(), CV_8UC1);
	cvtColor(image, processedImage, COLOR_BGR2GRAY);
	//resize(processedImage, processedImage, Size(processedImage.cols*2, processedImage.rows*2));

	//Codigo para aplicacion de filtro Gausiano  
	GaussianBlur(processedImage, processedImage, Size(3, 3), 0);
	if (SHOW_IMAGE)
	{
		namedWindow("GaussianBlur", CV_WINDOW_AUTOSIZE);
		imshow("GaussianBlur", processedImage);
		waitKey(0);
		destroyAllWindows();
	}

	//Codigo para aplicacion de filtro binario
	adaptiveThreshold(processedImage, processedImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 9, 11);

	//Canny(processedImage, processedImage, 50, 150);

	//Codigo para invertir colores binarios 
	//bitwise_not(processedImage, processedImage);

	// Codigo para dilatar o erosionar la imagen binaria, no parece necesario de momento pero dejo el codigo por si acaso
	
	Mat kernel;
	kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	//dilate(processedImage, processedImage, kernel);
	//erode(processedImage, processedImage, kernel);
	/*
	
	kernel = getStructuringElement(MORPH_CROSS, Size(2, 2));
	erode(processedImage, processedImage, kernel);
	
	*/


	if (SHOW_IMAGE)
	{
		namedWindow("adaptiveThreshold", CV_WINDOW_AUTOSIZE);
		imshow("adaptiveThreshold", processedImage);
		waitKey(0);
		destroyAllWindows();
	}


	//Se clona la imagen por si en algun momento se quiere usar processedImage para algo, enseñarla al final codigo o lo que sea
	Mat startImg = processedImage.clone();




	//almacena en el vector contours los vertices de cada uno de los contorno encontrados

	vector<vector<Point> > contours;
	findContours(startImg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	// ordena los contornos de mayor a menos area
	sort(contours.begin(), contours.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
		return contourArea(c1, false) > contourArea(c2, false);
	});

	//Se itera hasta encontrar en contorno de mayor area (p1) y el segundo de mayor area (p2), se filtra para que no busque en areas de menos de 25px (creo que son px)

	double area; 
	

	vector<ImageContour> outContours;

	// filtra los contornos de area menor a MIN_CELL_AREA
	for (int i = 0; i < contours.size(); i++)
	{
		area = contourArea(contours[i], false);
		if (area > MIN_CELL_AREA)
		{
			ImageContour validContour(contours[i]);
			outContours.push_back(validContour);
		}
	}

	
	if (SHOW_IMAGE)
	{
		Mat contourImage = processedImage.clone();
		cvtColor(contourImage, contourImage, COLOR_GRAY2BGR);

		vector<vector<Point>> printContour;
		printContour.push_back(outContours[0].getPoints());
		drawContours(contourImage, printContour, 0, Scalar(255, 0, 0), 1, 8);
		printContour.clear();

		namedWindow("External Contour", CV_WINDOW_AUTOSIZE);
		imshow("External Contour", contourImage);
		waitKey(0);
		destroyAllWindows();

		contourImage = processedImage.clone();
		cvtColor(contourImage, contourImage, COLOR_GRAY2BGR);

		for (int i = 1; i < outContours.size(); i++) {
			printContour.push_back(outContours[i].getPoints());
			drawContours(contourImage, printContour, 0, Scalar(255, 0, 0), 1, 8);
			printContour.clear();
		}
		
		namedWindow("Cell Contours", CV_WINDOW_AUTOSIZE);
		imshow("Cell Contours", contourImage);
		waitKey(0);
		destroyAllWindows();
	}


	//outContours.push_back(extContour);
	//outContours.push_back(cellContour);

	return outContours;

}


int calculateRows(ImageContour wordSearchContour , ImageContour cellContour , double error) {

	int rows = wordSearchContour.getHeight() / (cellContour.getHeight() *(1 + error))+0.5;
	return rows;

}

int calculateCols(ImageContour wordSearchContour, ImageContour cellContour, double error) {

	int cols = wordSearchContour.getWidth() / (cellContour.getWidth() *(1+error)) + 0.5;
	return cols;

}


Mat extractCellImage(Mat wordSearchImage, ImageContour cellContour, bool SHOW_IMAGE) {

	Mat cellImage;
	double cellHeight = cellContour.getHeight();
	double cellWidth = cellContour.getWidth();
	int x = cellContour.getPoints()[0].x;
	int y = cellContour.getPoints()[0].y;
	cellImage = Mat(wordSearchImage, Rect(x + 0.10*cellWidth,y + 0.10*cellWidth, cellWidth - 0.10*cellWidth, cellHeight - 0.10*cellWidth));
	
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

Ptr<cv::ml::KNearest> loadOCR() {

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

	return kNearest;
}

char OCR(Mat image , Ptr<cv::ml::KNearest> kNearest,bool DEBUG_MODE) {
	vector<DataContour> allContoursWithData;           // declare empty vectors,
	vector<DataContour> validContoursWithData;         // we will fill these shortly




	// TEST

	Mat matTestingNumbers = image.clone();            // read in the test numbers image
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
		namedWindow("Scaled GaussianBlur character", CV_WINDOW_AUTOSIZE);
		imshow("Scaled GaussianBlur character", matBlurred);
		waitKey(0);
		destroyAllWindows();
	}

	adaptiveThreshold(matBlurred, matThresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 3, 5);

	Mat kernel;
	kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(matThresh, dilatedImage, kernel);

	matThreshCopy = dilatedImage.clone();              // make a copy of the thresh image, this in necessary b/c findContours modifies the image

	if (DEBUG_MODE)
	{
		namedWindow("Threshold character", CV_WINDOW_AUTOSIZE);
		imshow("Threshold character", matThreshCopy);
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

	Mat matROIResized;

	for (int i = 0; i < validContoursWithData.size(); i++) {            // for each contour

		rectangle(matTestingNumbers, validContoursWithData[i].getBoundingRect(), Scalar(0, 255, 0), 2);

		Mat matROI = matThresh(validContoursWithData[i].getBoundingRect());          // get ROI image of bounding rect

		//Mat matROIResized;
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
		      // show the full string

		namedWindow("bounding character", CV_WINDOW_AUTOSIZE);
		imshow("bounding character", matTestingNumbers);     // show input image with green boxes drawn around found digits

		waitKey(0);
		destroyAllWindows();
		// wait for user key press

		namedWindow("OCR image", CV_WINDOW_AUTOSIZE);
		imshow("OCR image", matROIResized);     // show input image with green boxes drawn around found digits

		waitKey(0);
		destroyAllWindows();

		cout << "\n\n" << "numbers read = " << strFinalString << "\n\n";
	}

	return strFinalString[0];
}

Mat roundWord(Mat roundImageOut, vector<vector<ImageContour>> contourCellsMatriz , vector<Point> solution, bool DEBUG_MODE)
{


	bool TEXT_MODE = false;
	bool rectangulo = false;
	bool elipse = true;

	int initialX, initialY, finalX, finalY;
	int wordSize = solution.size() - 1;

	Point initial = solution[0];
	int initX = initial.x;
	int initY = initial.y;

	Point firstCharCentralPoint;
	firstCharCentralPoint.x = (contourCellsMatriz[initX][initY].getPoints()[0].x + contourCellsMatriz[initX][initY].getPoints()[2].x) / 2;
	firstCharCentralPoint.y = (contourCellsMatriz[initX][initY].getPoints()[0].y + contourCellsMatriz[initX][initY].getPoints()[2].y) / 2;

	Point final = solution[wordSize];
	int endX = final.x;
	int endY = final.y;

	Point LastCharCentralPoint;
	LastCharCentralPoint.x = (contourCellsMatriz[endX][endY].getPoints()[0].x + contourCellsMatriz[endX][endY].getPoints()[2].x) / 2;
	LastCharCentralPoint.y = (contourCellsMatriz[endX][endY].getPoints()[0].y + contourCellsMatriz[endX][endY].getPoints()[2].y) / 2;

	int rows = contourCellsMatriz.size();
	int cols = contourCellsMatriz[0].size();
	double cellHeight = contourCellsMatriz[0][0].getHeight();
	double cellWidth = contourCellsMatriz[0][0].getWidth();

	Point verticeIzqSup;
	Point verticeDerSup;
	Point verticeDerInf;
	Point verticeIzqInf;

	if (solution[0].y == solution[wordSize].y && solution[0].x < solution[wordSize].x) {
		//VERTICAL SENTIDO NORMAL
		verticeIzqSup = contourCellsMatriz[initX][initY].getPoints()[0];
		verticeDerSup = contourCellsMatriz[initX][initY].getPoints()[1];
		verticeDerInf = contourCellsMatriz[endX][endY].getPoints()[2];
		verticeIzqInf = contourCellsMatriz[endX][endY].getPoints()[3];
	}

	else if (solution[0].y == solution[wordSize].y && solution[0].x > solution[wordSize].x) {
		//VERTICAL SENTIDO INVERTIDO
		 verticeIzqSup = contourCellsMatriz[endX][endY].getPoints()[0];
		 verticeDerSup =contourCellsMatriz[endX][endY].getPoints()[1];
		 verticeDerInf =contourCellsMatriz[initX][initY].getPoints()[2];
		 verticeIzqInf = contourCellsMatriz[initX][initY].getPoints()[3];
	}

	else if (solution[0].x == solution[wordSize].x && solution[0].y < solution[wordSize].y) {
		//HORIZONTAL SENTIDO NORMAL
		 verticeIzqSup = contourCellsMatriz[initX][initY].getPoints()[0];
		 verticeDerSup = contourCellsMatriz[endX][endY].getPoints()[1];
		 verticeDerInf = contourCellsMatriz[endX][endY].getPoints()[2];
		 verticeIzqInf = contourCellsMatriz[initX][initY].getPoints()[3];
	}
	else if (solution[0].x == solution[wordSize].x && solution[0].y > solution[wordSize].y) {
		//HORIZONTAL SENTIDO INVERTIDO
		 verticeIzqSup = contourCellsMatriz[endX][endY].getPoints()[0];
		 verticeDerSup = contourCellsMatriz[initX][initY].getPoints()[1];
		 verticeDerInf = contourCellsMatriz[initX][initY].getPoints()[2];
		 verticeIzqInf = contourCellsMatriz[endX][endY].getPoints()[3];

	}
	else if(solution[0].x > solution[wordSize].x && solution[0].y < solution[wordSize].y){
		//DIAGONAL DERECHA ARRIBA SENTIDO NORMAL
		 verticeIzqSup.y = contourCellsMatriz[endX][endY].getPoints()[0].y ;
		 verticeIzqSup.x = contourCellsMatriz[endX][endY].getPoints()[0].x + cellHeight / 3;
		 verticeDerSup.y = contourCellsMatriz[endX][endY].getPoints()[2].y - cellHeight / 3;
		 verticeDerSup.x = contourCellsMatriz[endX][endY].getPoints()[2].x;
		 verticeDerInf.y = contourCellsMatriz[initX][initY].getPoints()[2].y;
		 verticeDerInf.x = contourCellsMatriz[initX][initY].getPoints()[2].x - cellHeight / 3;
		 verticeIzqInf.y = contourCellsMatriz[initX][initY].getPoints()[0].y + cellHeight / 3;
		 verticeIzqInf.x = contourCellsMatriz[initX][initY].getPoints()[0].x;

	}
	else if (solution[0].x < solution[wordSize].x && solution[0].y > solution[wordSize].y) {
		//DIAGONAL DERECHA ARRIBA SENTIDO INVERTIDO
		 verticeIzqSup.y = contourCellsMatriz[initX][initY].getPoints()[0].y;
		 verticeIzqSup.x = contourCellsMatriz[initX][initY].getPoints()[0].x + cellHeight / 3;
		 verticeDerSup.y = contourCellsMatriz[initX][initY].getPoints()[2].y - cellHeight / 3;
		 verticeDerSup.x = contourCellsMatriz[initX][initY].getPoints()[2].x;
		 verticeDerInf.y = contourCellsMatriz[endX][endY].getPoints()[2].y;
		 verticeDerInf.x = contourCellsMatriz[endX][endY].getPoints()[2].x - cellHeight / 3;
		 verticeIzqInf.y = contourCellsMatriz[endX][endY].getPoints()[0].y + cellHeight / 3;;
		 verticeIzqInf.x = contourCellsMatriz[endX][endY].getPoints()[0].x;
	}
	else if(solution[0].x < solution[wordSize].x && solution[0].y < solution[wordSize].y) {
		//DIAGONAL IZQUIERDA ARRIBA SENTIDO NORMAL
		 verticeIzqSup.y = contourCellsMatriz[initX][initY].getPoints()[3].y - cellHeight / 3;
		 verticeIzqSup.x = contourCellsMatriz[initX][initY].getPoints()[3].x;
		 verticeDerSup.y = contourCellsMatriz[initX][initY].getPoints()[1].y;
		 verticeDerSup.x = contourCellsMatriz[initX][initY].getPoints()[1].x - cellWidth / 3;
		 verticeDerInf.y = contourCellsMatriz[endX][endY].getPoints()[1].y + cellHeight / 3;
		 verticeDerInf.x = contourCellsMatriz[endX][endY].getPoints()[1].x;
		 verticeIzqInf.y = contourCellsMatriz[endX][endY].getPoints()[3].y;
		 verticeIzqInf.x = contourCellsMatriz[endX][endY].getPoints()[3].x + cellWidth / 3;
	}
	else if(solution[0].x > solution[wordSize].x && solution[0].y > solution[wordSize].y) {
		//DIAGONAL IZQUIERDA ARRIBA SENTIDO INVERTIDO
		 verticeIzqSup.y = contourCellsMatriz[endX][endY].getPoints()[3].y - cellHeight/3;
		 verticeIzqSup.x = contourCellsMatriz[endX][endY].getPoints()[3].x ;
		 verticeDerSup.y = contourCellsMatriz[endX][endY].getPoints()[1].y ;
		 verticeDerSup.x = contourCellsMatriz[endX][endY].getPoints()[1].x - cellWidth / 3;
		 verticeDerInf.y = contourCellsMatriz[initX][initY].getPoints()[1].y + cellHeight / 3;
		 verticeDerInf.x = contourCellsMatriz[initX][initY].getPoints()[1].x;
		 verticeIzqInf.y = contourCellsMatriz[initX][initY].getPoints()[3].y;
		 verticeIzqInf.x = contourCellsMatriz[initX][initY].getPoints()[3].x + cellWidth / 3;
	}

	Scalar color(
		(int)std::rand() % 255,
		(int)std::rand() % 255, 
		(int)std::rand() % 255
	);

	vector<Point> rectangle;
	rectangle.push_back(verticeIzqSup);
	rectangle.push_back(verticeDerSup);
	rectangle.push_back(verticeDerInf);
	rectangle.push_back(verticeIzqInf);

		
	polylines(roundImageOut, rectangle,true ,color, 2, LINE_8);

	if (DEBUG_MODE)
	{
		namedWindow("roundImageOut", CV_WINDOW_AUTOSIZE);
		imshow("roundImageOut", roundImageOut);
		waitKey(0);
		destroyAllWindows();
	}

	return roundImageOut;
}

Mat roundWord(Mat roundsImage, vector<Point> solution, int rows, int cols, double cellWidth, double cellHeight, bool DEBUG_MODE)
{
	Mat roundImageOut = roundsImage.clone();

	bool TEXT_MODE = false;
	bool rectangulo = false;
	bool elipse = true;

	int initialX, initialY, finalX, finalY;
	int wordSize = solution.size() - 1;

	Point initial = solution[0];
	int initX = initial.y;
	int initY = initial.x;

	Point final = solution[wordSize];
	int endX = final.y;
	int endY = final.x;
	if (TEXT_MODE) {
		cout << "\n point ini: " << solution[0] << endl;
		cout << "\n point end: " << solution[wordSize] << endl;
		cout << "\n pos initX: " << initX << "\n pos initY: " << initY << endl;
		cout << "\n pos endX: " << endX << "\n pos endY: " << endY << endl;
		cout << "\n word size: " << wordSize << endl;
	}
	//para añadir los anchos de celda de forma adecuada sin importar la dirección de la palabra
	//Establecemos que el punto inicial siempre es el que está mas a la izquierda
	if (initX < endX) {
		initialX = initX;
		finalX = endX;
		initialY = initY;
		finalY = endY;
	}
	else {
		initialX = endX;
		finalX = initX;
		initialY = endY;
		finalY = initY;
	}

	//desfase vertical para corregir altura
	int desfaseY = finalX + finalY;

	//Coordenadas del centro de la celda inicial y final
	double coordX1 = (initialX*cellWidth + cellWidth / 2);
	double coordX2 = (finalX*cellWidth + cellWidth / 2);
	double coordY1 = (initialY*cellWidth + cellWidth / 2);
	double coordY2 = (finalY*cellWidth + cellWidth / 2);

	//Para que cambie cada iteración
	/*std::srand(std::time(0));
	double red = rand() % 1;
	double green = rand() % 1;
	double blue = rand() % 1;*/

	//sin el * red etc.. solo salen colores grises, con el * solo blancos¿?¿?¿?¿?
	cv::Scalar color(
		(int)std::rand() % 255,
		(int)std::rand() % 255,
		(int)std::rand() % 255
	);
	if (TEXT_MODE) {
		cout << "\n" << color << endl;
	}
	//Esta parte es para dibujar cuadrados. No está acabado y cuadrarlo es complicado
	if (rectangulo){
		//cv::Point coordIni(coordX1, coordY1);
		//cv::Point coordFinal(coordX2, coordY2);
		//Estos valores dependerán de la orientación de la palabra para colocar los vértices de forma correcta
		int vX1, vX2, vY1, vY2;
		
		//HORIZONTAL
		if (initialX == finalX) {
			vX1 = -(cellWidth / 2 + 2);
			vX2 = cellWidth / 2 + 2;
			vY1 = -(cellHeight / 2 + 2) - desfaseY;
			vY2 = cellHeight / 2 + 2 - desfaseY;
		}//VERTICAL
		else {
			vX1 = -(cellHeight / 2 + 2);
			vX2 = cellHeight / 2 + 2;
			vY1 = -(cellWidth / 2 + 2) - desfaseY;
			vY2 = cellWidth / 2 + 2 - desfaseY;
		}


		cv::Point verticeIzqSup(coordX1+vX1, coordY1+vY1);
		cv::Point verticeDerInf(coordX2+vX2, coordY2+vY2);

		if (TEXT_MODE) {
			cout << "\n coordenada X inicial: " << coordX1 << "\n coordenada Y inicial: " << coordY1 << endl;
			cout << "\n coordenada X final: " << coordX2 << "\n coordenada Y final: " << coordY2 << endl;
			cout << "\n Point ini: " << verticeIzqSup << endl;
			cout << "\n Point final: " << verticeDerInf << endl;
		}
		rectangle(roundImageOut, verticeIzqSup, verticeDerInf, color, 2, LINE_8);
	}
	//Esta parte es para obtener el punto central de la elipse

	//Para dar diferentes colores a cada palabra
	if (elipse) {
		
		//int desfaseY = pow(finalX/10 + finalY/10,2);
		double desfaseY = 2.5 * initialY + initialX/4;

		double centerX = (coordX1 + coordX2) / 2;
		double centerY = (coordY1 + coordY2) / 2 - desfaseY; //el desfase parece aumentar cuanto más lejos del origen
		cv::Point coordCenter(centerX, centerY);
		int sizeX = wordSize * cellWidth / 2 + 20;
		int sizeY = cellWidth / 2 + 3 + wordSize/2;
		int angle=90;

		if (initialY == finalY) {
			angle = 0;
		}
		else if (initialX == finalX) {
			angle = 90;
		}//no pilla este ángulo, no sé por qué
		else if (initialY > finalY & initialX != finalX) {
			angle = 360 - atan(rows / cols) * 180 / PI;
			sizeX = wordSize * cellWidth * sqrt(2) / 2 + 20;
		}
		else if (initialY < finalY & initialX != finalX) {
			angle = atan(rows / cols) * 180 / PI;
			sizeX = wordSize * cellWidth * sqrt(2) / 2 + 20;
		}
		Size axes(sizeX, sizeY);
		ellipse(roundImageOut, coordCenter, axes, angle, 0, 360, color, 2.5, LINE_8);

	}
	return roundImageOut;
}




vector<vector<ImageContour>> orderCellContour(ImageContour extContour, vector<ImageContour> cellContours) {
	
	vector<ImageContour> cellContoursList = cellContours;
	double minHeight= cellContoursList[0].getHeight();
	double minWidth = cellContoursList[0].getWidth();
	vector<vector<ImageContour>> cellContourMatrix;
	vector<ImageContour> cellContourRow;

	for (int i = 1; i < cellContoursList.size(); i++) {
		if (cellContoursList[i].getHeight() < minHeight) {
			minHeight = cellContoursList[i].getHeight();
		}
		if (cellContoursList[i].getWidth() < minWidth) {
			minWidth = cellContoursList[i].getWidth();
		}
	}

	
	
	int row = 0;

	bool findHorizontal = true;
	bool findVertical = true;

	double limitX = extContour.getPoints()[0].x + minWidth / 2;
	double limitY = extContour.getPoints()[0].y + minHeight / 2;

	while (findVertical) {
		
		while (findHorizontal) {
			for (int i = 0; i < cellContoursList.size(); i++) {
				//int x =  cellContoursList[i].getPoints()[1].x;
				//int y =  cellContoursList[i].getPoints()[3].y;
				if (cellContoursList[i].getPoints()[0].y <= limitY && cellContoursList[i].getPoints()[0].x < limitX) {
					cellContourRow.push_back(cellContoursList[i]);
					limitX = minWidth/2 + cellContoursList[i].getPoints()[1].x;
					limitY = minHeight / 2 + cellContoursList[i].getPoints()[1].y;
					cellContoursList.erase(cellContoursList.begin()+i);
					findHorizontal = true;
					findVertical = true;

					//double width = cellContoursList[i].getWidth();
					
					break;
				}
				else if (cellContoursList[i].getPoints()[0].y < limitY) {
					findVertical = false;
				}
				else if (cellContoursList[i].getPoints()[0].x < limitX) {
					findHorizontal = false;
				}
			}
			if (cellContoursList.size() == 0) {
				findVertical = false;
				findHorizontal = false;
			}
		}
		cellContourMatrix.push_back(cellContourRow);
		limitY = minHeight/2 + cellContourRow[0].getPoints()[3].y;
		limitX = minWidth /2 + cellContourRow[0].getPoints()[3].x;
		//limitX = extContour.getPoints()[0].x + minWidth;
		cellContourRow.clear();
		row = row + 1;
		findHorizontal = true;

		
		
	}

	return cellContourMatrix;


}
