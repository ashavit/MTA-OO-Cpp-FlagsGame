#pragma once

#include <stdio.h>
#include "Cell.h"

class Cell;

#define DEFAULT_BOARD_SIZE 13

class Board {
    size_t  height;
    size_t  width;
    Cell*** board;

	enum BoardDensity { LOW = 10, REGULAR = 20, HIGH = 30 };

    Board() : Board(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE) {}
    Board(size_t width, size_t height);
    
    void randomPlaceSpecialCells(CellType type, int count);
    
public:
    
    static Board* loadRandomBoard() { return Board::loadRandomBoard(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);};
    static Board* loadRandomBoard(size_t width, size_t height);
    
    ~Board();

    Cell* getRandomCellInRows(size_t from, size_t to);

    Cell* getCellAtPosition(size_t row, size_t column) {
        return board[column][row];
    }

    Cell* getNextCell(const Cell* cell, Direction direction);
    
    void printBoard();
    void drawBoard();
	void drawCell(Cell* cell);

};
