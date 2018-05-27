#ifndef _ENTITY_H
#define _ENTITY_H
#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>

#define dim 8

using namespace std;

int checkSign(int);

class Entity{
	protected:
		int currentPlayer;
		int dimension;
		int** board;
	public:
		int getDimension();
		int getSpotStatus(int, int);
		void setSpotStatus(int, int, int);
		void getBoard();
		int** getBoardData() { return board; };
		int getCurrentPlayer();
		void changeCurrentPlayer();
		void info();
		bool scoreBoard();
		
		Entity();
		Entity(Entity&);
		~Entity();
		
		bool checkDimension(int, int, int, int);
		bool checkDimension(int, int);
		bool checkRange(int, int, int, int);
				
		bool checkBeatingWhite();
		bool checkBeatingWhiteDame();
		bool checkBeatingBlack();
		bool checkBeatingBlackDame();
		
		bool checkBeatingWhite(int, int);
		bool checkBeatingWhiteDame(int, int);
		bool checkBeatingBlack(int, int);
		bool checkBeatingBlackDame(int, int);

		bool checkMoveWhite(int, int, int, int);
		bool checkMoveWhiteDame(int, int, int, int);
		bool checkMoveBlack(int, int, int, int);
		bool checkMoveBlackDame(int, int, int, int);
		
		bool checkMove(int, int, int, int);
		
};
#endif

