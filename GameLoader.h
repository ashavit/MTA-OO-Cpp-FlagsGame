#pragma once

#include <stdio.h>
#include <string>
#include <list>
#include "Board.h"

class Player;
class Board;
class PlayerMoves;

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
	Board* loadBoardFromFile(std::ifstream& boardFile, const std::string& fileName) {
		return loadBoardFromFile(boardFile, fileName, Board::DEFAULT_BOARD_SIZE, Board::DEFAULT_BOARD_SIZE); 
	}
	Board* loadBoardFromFile(std::ifstream& boardFile, const std::string& fileName, UINT width, UINT height);
	PlayerMoves* loadPlayerMovesFromFile(std::ifstream& movesFile);

	void saveMovesToFile(std::ofstream& movesFile, PlayerMoves& moves, int roundToMod2);

	std::ifstream* openFileToRead(const std::string fileName);
	std::ofstream* openFileToWrite(const std::string fileName);
	void closeAndReleaseFile(std::ifstream* file);
	void closeAndReleaseFile(std::ofstream* file);
		
	void addUniqueError(const std::string& error);

public:
	/* Methods take in Players to support reverse games */
	GameLoader(Player& playerA, Player& playerB);
	~GameLoader();
	static int aliveInstances() { return aliveIns; }

	bool loadRandomGame();
	bool loadGameFromFile(const std::string& fileName);
	void printErrors();

	Board* gameBoard() { return _gameBoard; }

	void savePlayerMovesToFile(const std::string& fileName);
};

