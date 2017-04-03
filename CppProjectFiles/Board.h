#ifndef FLAGSGAME_BOARD_H
#define FLAGSGAME_BOARD_H


#include "Cell.h"

class Board {
    size_t  hight;
    size_t  width;
    Cell*** board;

public:
    Board() : Board(13, 13) {}
    Board(size_t hight, size_t width) : hight(hight), width(width) {
        board = new Cell**[hight];
        for (size_t i = 0; i < hight; ++i) {
            board[i] = new Cell*[width];
            for (size_t j = 0; j < width; ++j) {
                board[i][j] = new Cell(i,j);
            }
        }
    }

    Cell& getCellatPosition(size_t row, size_t column) {
        return *board[row][column];
    }
};


#endif //FLAGSGAME_BOARD_H
