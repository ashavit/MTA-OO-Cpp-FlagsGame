#pragma once

#include "Cell.h"

class Cell;
class GameLoader;

class Board
{
	static int aliveIns;
	UINT height;
	UINT width;
	Cell*** board;

	enum BoardDensity
	{
		LOW = 10,
		REGULAR = 20,
		HIGH = 30
	};

	Board() : Board(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE) {}
	Board(UINT width, UINT height);
	Board(Board const&) = delete;
	void operator=(Board const&) = delete;

	void randomPlaceSpecialCells(CellType type, int count) const;

	friend GameLoader;

public:
	static const int DEFAULT_BOARD_SIZE = 13;

	static int aliveInstances() { return aliveIns; }

	~Board();

	Cell* getRandomCellInRows(size_t from, size_t to) const;

	Cell* getNextCell(const Cell* cell, Direction direction) const;

	void printBoard() const;
	void drawBoard() const;
	void drawCell(Cell* cell) const;
	void printMessage(const std::string message, bool onFullScreen, bool waitForResponse) const;
	int getPlayerStatsLocation() const;
};
