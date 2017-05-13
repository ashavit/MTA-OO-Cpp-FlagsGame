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

	Board* loadRandomBoard(UINT width = Board::DEFAULT_BOARD_SIZE, UINT height = Board::DEFAULT_BOARD_SIZE) const;
	Board* loadBoardFromFile(std::ifstream& boardFile, const std::string& fileName, UINT width = Board::DEFAULT_BOARD_SIZE, UINT height = Board::DEFAULT_BOARD_SIZE);
	PlayerMoves* loadPlayerMovesFromFile(std::ifstream& movesFile);

	void saveBoardToFile(std::ofstream& boardFile) const;
	void saveMovesToFile(std::ofstream& movesFile, PlayerMoves& moves, int roundToMod2) const;

	std::ifstream* openFileToRead(const std::string fileName) const;
	std::ofstream* openFileToWrite(const std::string fileName) const;
	void closeAndReleaseFile(std::ifstream* file) const;
	void closeAndReleaseFile(std::ofstream* file) const;

	void addUniqueError(const std::string& error);

public:
	/* Methods take in Players to support reverse games */
	GameLoader(Player& playerA, Player& playerB);
	~GameLoader();
	GameLoader(GameLoader const&) = delete;
	void operator=(GameLoader const&) = delete;

	static int aliveInstances() { return aliveIns; }

	bool loadRandomGame();
	bool loadGameFromFile(const std::string& fileName);
	bool loadGameMovesFromFile(const std::string& fileName);
	void printErrors();

	Board* gameBoard() const { return _gameBoard; }

	std::string newRandomFileName() const;
	void saveBoardToFile(const std::string& fileName) const;
	void savePlayerMovesToFile(const std::string& fileName) const;
};

