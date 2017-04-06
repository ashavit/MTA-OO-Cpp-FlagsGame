#ifndef FLAGSGAME_BOARD_H
#define FLAGSGAME_BOARD_H


#include "Cell.h"
#include "Utils.h"

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

    void draw() {
        Cell* cell;
        Ship* standingShip;
        for (int row = 0; row < height; ++row) {
            for (int column = 0; column < width; ++column) {
                cell = board[row][column];
                standingShip = cell->getStandingShip();
                switch (cell->getCellType()){
                    case SEA:
                        //setTextColor(WHITE, BLUE);
                        break;
                    case FORREST:
                        //setTextColor(WHITE, GREEN);
                        break;
                    case FLAG_A:
                        //setTextColor(WHITE, RED);
                        break;
                    case FLAG_B:
                        //setTextColor(WHITE, YELLOW);
                        break;
                    case REGULAR:
                    default:
                        //setTextColor(WHITE, BLACK);
                        break;
                }
                std::cout << (standingShip != nullptr ? standingShip->getShipType() : ' ');
            }
            std::cout << std::endl;
        }
    }
};


#endif //FLAGSGAME_BOARD_H
