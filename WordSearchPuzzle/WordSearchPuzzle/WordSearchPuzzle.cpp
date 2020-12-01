

#include "WordSearchPuzzle.h"

WordSearchPuzzle::WordSearchPuzzle(vector<vector<char>> wordSearchMatrix, vector<String> words)
{
	this->wordSearchMatrix = wordSearchMatrix;
	this->words = words;
	this->rows = wordSearchMatrix[0].size();
	this->cols = wordSearchMatrix.size();

}

void WordSearchPuzzle::printConsole() {
	for (auto fila = 0; fila < wordSearchMatrix.size(); fila++) {
		for (int celda = 0; celda < wordSearchMatrix[fila].size(); celda++) {
			cout << wordSearchMatrix[fila][celda] << " ";

		}
		cout << "\n";
	}
}

void WordSearchPuzzle::solve()
{
	for (int i = 0; i < words.size(); i++) {
		vector<Point> wordSolution = findWord(words[i]);
		if (wordSolution.size() != 0)
		{
			solution.push_back(wordSolution);
			cout << "Palabra " << words[i] << " encontrada \n";
		}
	}
}


vector<Point> WordSearchPuzzle::findWord(String word) {
	
	vector<Point> foundWord;
	char firstCharacter = word[0];
	// Se va recorriendo la sopa de letras buscando coincidencias con la primera letra
	for (int m = 0; m < rows; m++) {
		for (int n = 0; n < cols; n++) {
			// Si se encuentra una letra coincidente, se buscan en las posiciones vecinas coincidencias con la segunda letra
			if (firstCharacter == wordSearchMatrix[m][n]) {
				Point foundCharacter1;
				foundCharacter1.x = m;
				foundCharacter1.y = n;
				char secondCharacter = word[1];
				vector<Point> neighbourCharacterList = lookNeighbours(foundCharacter1, secondCharacter);
				if (neighbourCharacterList.size() != 0) {

				
					// si se encuentra coincidencias con las letras vecinas, para coincidencia, se calcula la direccion de la palabra y se busca en esa direccion
					for (int j = 0; j < neighbourCharacterList.size(); j++) {
						Point foundCharacter2;
						foundCharacter2.x = neighbourCharacterList[j].x;
						foundCharacter2.y = neighbourCharacterList[j].y;
						int direction = calculateDireccion(foundCharacter1, foundCharacter2);
						vector<Point> foundCharacterList;
						//Se busca en la direccion hasta que encontrar todas las letras o encontrar una que no coincida
						Point foundCharacterPrev = foundCharacter2;
						for (int k = 2; k < word.size(); k++) {
							Point foundCharacterNext = LookDirection(direction, foundCharacterPrev, word[k]);


							if (foundCharacterNext.x == -1) {
								break;
							}
							foundCharacterList.push_back(foundCharacterNext);
							foundCharacterPrev = foundCharacterNext;
							// si ya se han encontrada todas las palabras se devuelve el vector con las posiciones de la palabra en la sopa de letras
							if (k == word.size() - 1) {
								foundWord.push_back(foundCharacter1);
								foundWord.push_back(foundCharacter2);
								for (int l = 0; l < foundCharacterList.size(); l++) {
									foundWord.push_back(foundCharacterList[l]);
								}
								return foundWord;
							}



						}
					}


				}

			}

		}
		
	}

	return foundWord;
}

vector<vector<Point>> WordSearchPuzzle::getSolution()
{
	return this->solution;
}

vector<Point> WordSearchPuzzle::lookNeighbours(Point pos, char foundCharacter) {
	
	vector<Point> foundPosList;
	if (pos.x != 0) {
		char character = wordSearchMatrix[pos.x-1][pos.y];
		if (character == foundCharacter) {
			Point foundPos;
			foundPos.x = pos.x - 1;
			foundPos.y = pos.y;
			foundPosList.push_back(foundPos);
		}
		if (pos.y != 0) {
			char character = wordSearchMatrix[pos.x - 1][pos.y - 1];
			if (character == foundCharacter) {
				Point foundPos;
				foundPos.x = pos.x - 1;
				foundPos.y = pos.y - 1;
				foundPosList.push_back(foundPos);
			}
		}
		
		if (pos.y != rows -1) {
			char character = wordSearchMatrix[pos.x - 1][pos.y + 1];
			if (character == foundCharacter) {
				Point foundPos;
				foundPos.x = pos.x - 1;
				foundPos.y = pos.y +1;
				foundPosList.push_back(foundPos);
			}
		}
	}
	if (pos.x != cols -1) {

		char character = wordSearchMatrix[pos.x + 1][pos.y];
		if (character == foundCharacter) {
			Point foundPos;
			foundPos.x = pos.x + 1;
			foundPos.y = pos.y;
			foundPosList.push_back(foundPos);
		}
		if (pos.y != 0) {
			char character = wordSearchMatrix[pos.x + 1][pos.y - 1];
			if (character == foundCharacter) {
				Point foundPos;
				foundPos.x = pos.x + 1;
				foundPos.y = pos.y - 1;
				foundPosList.push_back(foundPos);
			}
		}
		if (pos.y != rows-1) {
			char character = wordSearchMatrix[pos.x + 1][pos.y + 1];
			if (character == foundCharacter) {
				Point foundPos;
				foundPos.x = pos.x + 1;
				foundPos.y = pos.y + 1;
				foundPosList.push_back(foundPos);
			}
		}
	}
	if (pos.y != 0) {
		char character = wordSearchMatrix[pos.x ][pos.y - 1];
		if (character == foundCharacter) {
			Point foundPos;
			foundPos.x = pos.x ;
			foundPos.y = pos.y - 1;
			foundPosList.push_back(foundPos);
		}

	}
	if (pos.y != rows-1) {
		char character = wordSearchMatrix[pos.x][pos.y + 1];
		if (character == foundCharacter) {
			Point foundPos;
			foundPos.x = pos.x ;
			foundPos.y = pos.y + 1;
			foundPosList.push_back(foundPos);
		}
	}

	return foundPosList;

}

int  WordSearchPuzzle::calculateDireccion(Point pos1, Point pos2) {

	if ( pos2.y == pos1.y &  pos2.x < pos1.x)
		return TOP;
	if (pos2.y > pos1.y &  pos2.x < pos1.x)
		return TOP_RIGHT;
	if (pos2.y > pos1.y &  pos2.x == pos2.x)
		return RIGHT;
	if (pos2.y > pos1.y &  pos2.x > pos1.x)
		return BOTTOM_RIGHT;
	if (pos2.y = pos1.y &  pos2.x > pos2.x)
		return BOTTOM;
	if (pos2.y < pos1.y &  pos2.x > pos2.x)
		return BOTTOM_LEFT;
	if (pos2.y < pos1.y &  pos2.x == pos2.x)
		return LEFT;
	if (pos2.y < pos1.y &  pos2.x < pos2.x)
		return TOP_LEFT;
}

Point WordSearchPuzzle::LookDirection(int direction, Point pos, char foundCharacter) {
	
	Point foundPos(-1,-1);

	if (direction == TOP && pos.x!=0) {
		char character = wordSearchMatrix[pos.x-1][pos.y];
		if (character == foundCharacter) {
			foundPos.x = pos.x-1;
			foundPos.y = pos.y;
		}
	}
	if (direction == TOP_RIGHT && (pos.x != 0 || pos.y != rows-1)) {
		char character = wordSearchMatrix[pos.x - 1][pos.y+1];
		if (character == foundCharacter) {
			foundPos.x = pos.x-1;
			foundPos.y = pos.y+1;
		}
	}
	if (direction == RIGHT && pos.y != rows-1) {
		char character = wordSearchMatrix[pos.x ][pos.y+1];
		if (character == foundCharacter) {
			foundPos.x = pos.x;
			foundPos.y = pos.y+1;
		}
	}
	if (direction == BOTTOM_RIGHT &&( pos.y != rows-1 || pos.x!=cols-1)) {
		char character = wordSearchMatrix[pos.x + 1][pos.y +1];
		if (character == foundCharacter) {
			foundPos.x = pos.x+1;
			foundPos.y = pos.y+1;
		}
	}
	if (direction == BOTTOM && pos.x != cols-1) {
		char character = wordSearchMatrix[pos.x + 1][pos.y];
		if (character == foundCharacter) {
			foundPos.x = pos.x+1;
			foundPos.y = pos.y;
		}
	}
	if (direction == BOTTOM_LEFT && (pos.x != cols-1 || pos.y!=0)) {
		char character = wordSearchMatrix[pos.x +1][pos.y-1];
		if (character == foundCharacter) {
			foundPos.x = pos.x+1;
			foundPos.y = pos.y-1;
		}
	}
	if (direction == LEFT && pos.y != 0) {
		char character = wordSearchMatrix[pos.x][pos.y-1];
		if (character == foundCharacter) {
			foundPos.x = pos.x;
			foundPos.y = pos.y-1;
		}
	}
	if (direction == TOP_LEFT && ( pos.x != 0  ||  pos.y != 0)) {
		char character = wordSearchMatrix[pos.x - 1][pos.y-1];
		if (character == foundCharacter) {
			foundPos.x = pos.x-1;
			foundPos.y = pos.y-1;
		}
	}
		


		return foundPos;
}
