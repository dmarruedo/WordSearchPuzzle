#include <iostream>
#include "opencv\cv.hpp"
#include<sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>

using namespace cv;
using namespace std;

struct WordPos
{
	int Xpos;
	int Ypos;
	int Xdir;
	int Ydir;
	int wordSize;

};
//std::vector<vector<char>> sopaLetras
struct WordPos WordSearch(vector<vector<char>>sopaLetras, int columns, int rows)
{
	bool wordFound = false;
	struct WordPos values;

	cout<< "\n" << "Ingrese la palabra que est� buscando en la sopa de letras" << "\n";
	string palabra;
	cin >> palabra;
	cout << "\n" << "Est� buscando la palabra: " << palabra << "\n";

	int size;
	size = palabra.length;

	char caracteres(size - 1);
	int pos = 0;

	string checkWord;

	for (int i = 0; i <= columns; i++) 
	{
		for (int j = 0; j <= rows; j++) 
		{
			//encontramos la primera letra de la palabra que estamos buscando
			if (palabra[pos] == sopaLetras[i][j])
			{
				//comprobamos si la palabra entra en la direcci�n estimada
				//de izquierda a derecha
				if (size < columns - i)
				{
					//si la palabra entra en la matriz, comprobamos 
					//primero creamos string con los caracteres de esa direcci�n y para ese tama�o
					for (int a = 0; a < size; a++) 
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
						values.wordSize = size;
						break;
						wordFound = true;
					}
				}
				//comprobamos de derecha a izquierda
				if (size <= i + 1)
				{
					for (int a = 0; a < size; a++)
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
						values.wordSize = size;
						break;
						wordFound = true;
					}
				}
				//de arriba a abajo
				if (size < rows - j) 
				{
					//si la palabra entra en la matriz, comprobamos 
					//primero creamos string con los caracteres de esa direcci�n y para ese tama�o
					for (int a = 0; a < size; a++)
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
						values.wordSize = size;
						break;
						wordFound = true;
					}
				}
				//comprobamos de abajo a arriba
				if (size <= j + 1)
				{
					for (int a = 0; a < size; a++)
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
						values.wordSize = size;
						break;
						wordFound = true;
					}
				}
			}
		}
		//para salir del segundo for
		if (wordFound)
		{
			break;
		}

	}

	return values;
};