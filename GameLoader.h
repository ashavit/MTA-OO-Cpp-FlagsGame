#pragma once

#include <list>
#include "Board.h"

class Board;
class PlayerMoves;
class FilePlayer;

class GameLoader
{
	static int aliveIns;

	Board* _gameBoard = nullptr;
	mutable std::list<std::string> errors;

	Board* loadRandomBoard(UINT width = Board::DEFAULT_BOARD_SIZE, UINT height = Board::DEFAULT_BOARD_SIZE) const;
	Board* loadBoardFromFile(std::ifstream& boardFile, const std::string& fileName, UINT width = Board::DEFAULT_BOARD_SIZE, UINT height = Board::DEFAULT_BOARD_SIZE) const;
	PlayerMoves* loadPlayerMovesFromFile(std::ifstream& movesFile) const;

	std::ifstream* openFileToRead(const std::string fileName) const;
	std::ofstream* openFileToWrite(const std::string fileName) const;
	void closeAndReleaseFile(std::ifstream* file) const;
	void closeAndReleaseFile(std::ofstream* file) const;

	void addUniqueError(const std::string& error) const;

public:
	/* Methods take in Players to support reverse games */
	GameLoader();
	~GameLoader();
	GameLoader(GameLoader const&) = delete;
	void operator=(GameLoader const&) = delete;

	static int aliveInstances() { return aliveIns; }

	bool loadRandomGame();
	bool loadGameFromFile(const std::string& fileName);
	bool loadGameMovesFromFile(const std::string& fileName, FilePlayer* playerA, FilePlayer* playerB) const;
	void printErrors() const;

	Board* gameBoard() const { return _gameBoard; }

};

