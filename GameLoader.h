#pragma once

#include <list>
#include "Board.h"

class Player;
class Board;
class PlayerMoves;

class GameLoader
{
	static int aliveIns;

	Player* playerA;
	Player* playerB;
	Board* _gameBoard = nullptr;
	mutable std::list<std::string> errors;

	Board* loadRandomBoard(UINT width = Board::cols, UINT height = Board::rows) const;
	Board* loadBoardFromFile(std::ifstream& boardFile, const std::string& fileName, UINT width = Board::cols, UINT height = Board::rows) const;
	PlayerMoves* loadPlayerMovesFromFile(std::ifstream& movesFile) const;

	void saveBoardToFile(std::ofstream& boardFile) const;
	void saveMovesToFile(std::ofstream& movesFile, PlayerMoves& moves, int roundToMod2) const;

	std::ifstream* openFileToRead(const std::string fileName) const;
	std::ofstream* openFileToWrite(const std::string fileName) const;
	void closeAndReleaseFile(std::ifstream* file) const;
	void closeAndReleaseFile(std::ofstream* file) const;

	void addUniqueError(const std::string& error) const;

public:
	/* Methods take in Players to support reverse games */
	GameLoader(Player* playerA, Player* playerB);
	~GameLoader();
	GameLoader(GameLoader const&) = delete;
	void operator=(GameLoader const&) = delete;

	static int aliveInstances() { return aliveIns; }

	bool loadRandomGame();
	bool loadGameFromFile(const std::string& fileName);
	bool loadGameMovesFromFile(const std::string& fileName) const;
	void printErrors() const;

	Board* gameBoard() const { return _gameBoard; }

	std::string newRandomFileName() const;
	void saveBoardToFile(const std::string& fileName) const;
	void savePlayerMovesToFile(const std::string& fileName) const;
};

