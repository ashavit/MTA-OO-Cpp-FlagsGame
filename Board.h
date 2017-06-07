#pragma once

#include <list>
#include "Cell.h"

class Cell;
class GameLoader;

class Board
{
	static int aliveIns;
	UINT height;
	UINT width;
	Cell*** board;
	std::list<Ship*> allShips;

	UINT rowLabelsSpace = 0;
	UINT scoreBoardIndentation = 10;

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
	void addedShip(Ship *ship);

	friend GameLoader;

public:
	static const int DEFAULT_BOARD_SIZE = 13;

	static int aliveInstances() { return aliveIns; }

	~Board();

	Cell* getRandomCellInRows(size_t from, size_t to) const;
	Cell* getNextCell(const Cell* cell, Direction direction) const;

	/* Coordinates start from 0,0 */
	Cell* getCellAt(int col, int row) const;
	/* Coordinates start from 0,0 */
	char charAt(int col, int row) const;

	void restartBoard() const;
	void drawBoard() const;
	void drawCell(Cell* cell) const;
	void printMessage(const std::string message, bool onFullScreen, int indent = 0, int rowsDown = 0) const;
	int getPlayerStatsLocation() const;
};
