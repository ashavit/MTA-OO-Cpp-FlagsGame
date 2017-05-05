#pragma once

#include <stdio.h>
#include <string>
#include <list>
#include "Board.h"

class Player;
class Board;

class GameLoader
{
	static int aliveIns;

	Player& playerB;
	Player& playerA;
	Board* _gameBoard = nullptr;
	std::list<std::string> errors;

	Board* loadRandomBoard() {
		return loadRandomBoard(Board::DEFAULT_BOARD_SIZE, Board::DEFAULT_BOARD_SIZE); 
	}
	Board* loadRandomBoard(UINT width, UINT height);
	Board* loadBoardFromFile(const std::string& fileName) {
		return loadBoardFromFile(fileName, Board::DEFAULT_BOARD_SIZE, Board::DEFAULT_BOARD_SIZE); 
	}
	Board* loadBoardFromFile(const std::string& fileName, UINT width, UINT height);

	void addUniqueErrorToList(std::list<std::string>& errors, const std::string& error);

public:
	/* Methods take in Players to support reverse games */
	GameLoader(Player& playerA, Player& playerB);
	~GameLoader();
	static int aliveInstances() { return aliveIns; }

	bool loadRandomGame();
	bool loadGameFromFile(const std::string& fileName);
	void printErrors();

	Board* gameBoard() { return _gameBoard; }

};

