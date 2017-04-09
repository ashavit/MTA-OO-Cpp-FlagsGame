#pragma once

#include <stdio.h>
#include "Cell.h"

class Cell;

#define DEFAULT_BOARD_SIZE 13

class Board {
    size_t  height;
    size_t  width;
    Cell*** board;

    Board() : Board(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE) {}
    Board(size_t height, size_t width);
    
    void randomPlaceSpecialCells(CellType type, int count);
    
public:
    
    static Board* loadRandomBoard() { return Board::loadRandomBoard(DEFAULT_BOARD_SIZE, DEFAULT_BOARD_SIZE);};
    static Board* loadRandomBoard(size_t height, size_t width);
    
    ~Board();

    Cell* getRandomCellInRows(size_t from, size_t to);

    Cell* getCellAtPosition(size_t row, size_t column) {
        return board[row][column];
    }

    Cell* getNextCell(const Cell* cell, Direction direction);
    
    void printBoard();
    void drawBoard();
};
