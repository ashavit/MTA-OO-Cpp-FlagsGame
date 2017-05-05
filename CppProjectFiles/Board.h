#pragma once

#include <stdio.h>
#include <list>
#include "Cell.h"

class Cell;
class GameLoader;

class Board {
	static int aliveIns;
    UINT  height;
    UINT  width;
    Cell*** board;

	enum BoardDensity { LOW = 10, REGULAR = 20, HIGH = 30 };

    Board() : Board(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE) {}
    Board(UINT width, UINT height);
    
    void randomPlaceSpecialCells(CellType type, int count);
    std::string newFileName(const std::string format);

	friend GameLoader;

public:
	static const int DEFAULT_BOARD_SIZE = 13;

	static int aliveInstances() { return aliveIns; }

    ~Board();

    Cell* getRandomCellInRows(size_t from, size_t to);

    Cell* getNextCell(const Cell* cell, Direction direction);
    
    void printBoard();
    void drawBoard();
	void drawCell(Cell* cell);
	void printMessage(const std::string message, bool onFullScreen, bool waitForResponse);
	int getPlayerStatsLocation();
    
    void saveToFile(std::string fileName);
};
