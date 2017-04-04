#ifndef FLAGSGAME_BOARD_H
#define FLAGSGAME_BOARD_H


#include "Cell.h"

class Board {
    size_t  height;
    size_t  width;
    Cell*** board;

public:
    Board() : Board(13, 13) {}
    Board(size_t height, size_t width) : height(height), width(width) {
        board = new Cell**[height];
        for (size_t i = 0; i < height; ++i) {
            board[i] = new Cell*[width];
            for (size_t j = 0; j < width; ++j) {
                board[i][j] = new Cell(i,j);
            }
        }
    }
    ~Board() {
        for (size_t i = 0; i < height; ++i) {
            for (size_t j = 0; j < width; ++j) {
                delete board[i][j];
            }
            delete board[i];
        }
        delete board;
    }

    Cell& getCellatPosition(size_t row, size_t column) {
        return *board[row][column];
    }
};


#endif //FLAGSGAME_BOARD_H
