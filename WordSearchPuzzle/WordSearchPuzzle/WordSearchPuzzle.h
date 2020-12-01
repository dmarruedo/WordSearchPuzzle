/*
 * Sudoku.h
 *
 *  Created on: 10 Oct 2015
 *      Author: guillaume
 */

#ifndef WORDSEARCHPUZZLE_H_
#define WORDSEARCHPUZZLE_H_


#include "opencv\cv.hpp"
#include <iostream>
using namespace std;
using namespace cv;

const int TOP = 0;
const int TOP_RIGHT = 1;
const int RIGHT = 2;
const int BOTTOM_RIGHT = 3;
const int BOTTOM = 4;
const int BOTTOM_LEFT = 5;
const int LEFT = 6;
const int TOP_LEFT = 7;


class WordSearchPuzzle{
	private:
		vector<vector<char>> wordSearchMatrix;
		vector<String> words;
		vector<vector<Point>> solution;
		int rows;
		int cols;


	public:
		WordSearchPuzzle(vector<vector<char>> wordSearchMatrix, vector<String> words);
		void printConsole();
		void solve();

		vector<Point> findWord(String word);
		
		vector<vector<Point>> getSolution();

		vector<Point> lookNeighbours(Point pos , char foundCharacter);

		int calculateDireccion(Point pos1, Point pos2);

		Point LookDirection(int direction, Point pos, char foundCharacter);



 };



#endif /* WORDSEARCHPUZZLE_H_ */

