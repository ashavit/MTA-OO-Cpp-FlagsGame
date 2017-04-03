#ifndef FLAGSGAME_CELL_H
#define FLAGSGAME_CELL_H


#include <iostream>
#include "Ship.h"

class Cell {
    enum CellType {REGULAR, FORREST, SEA, FLAG};

    size_t   row ;
    size_t   column;
    CellType type;
    Ship*    standingShip;

public:
    Cell(size_t row = 0, size_t column = 0, CellType type = REGULAR, Ship* standingShip = nullptr)
        : row(row), column(column), type(type), standingShip(standingShip)  {}

    CellType getCellType() {
        return type;
    }

    Ship& getStandingShip() {
        return *standingShip;
    }
};


#endif //FLAGSGAME_CELL_H
