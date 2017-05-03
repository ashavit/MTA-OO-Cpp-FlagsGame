#pragma once

#include <stdio.h>
#include <list>
#include "Cell.h"

class Cell;

#define DEFAULT_BOARD_SIZE 13

class Board {
	static int aliveIns;
    UINT  height;
    UINT  width;
    Cell*** board;

	enum BoardDensity { LOW = 10, REGULAR = 20, HIGH = 30 };

    Board() : Board(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE) {}
    Board(UINT width, UINT height);
    
    void randomPlaceSpecialCells(CellType type, int count);
    std::string newFileName(std::string format);
	void printErrors(std::list<std::string>& errors);

public:
    
    /* LoadBoard methods take in Players to randomize ship position, and support reverse games when loaded from files */
    static Board* loadRandomBoard(Player& playerA, Player& playerB) { return Board::loadRandomBoard(playerA, playerB, DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);};
    static Board* loadRandomBoard(Player& playerA, Player& playerB, UINT width, UINT height);
    static Board* loadBoardFromFile(Player& playerA, Player& playerB, const std::string& fileName) { return Board::loadBoardFromFile(playerA, playerB, fileName, DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);};
    static Board* loadBoardFromFile(Player& playerA, Player& playerB, const std::string& fileName, UINT width, UINT height);
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
