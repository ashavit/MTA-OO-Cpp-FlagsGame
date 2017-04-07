#pragma once

#include <iostream>
#include "Ship.h"

enum CellType {REGULAR, FORREST, SEA, FLAG_A, FLAG_B};

class Cell {

    size_t   row ;
    size_t   column;
    CellType type;
    Ship*    standingShip = nullptr;

    void setCellType(CellType aType) { type = aType; }
    
public:
    Cell(size_t row, size_t column, CellType type = REGULAR)
        : row(row), column(column), type(type)  { }

    CellType getCellType() const {
        return type;
    }

    void setStandingShip(Ship* ship) {
        standingShip = ship;
    }

    Ship* getStandingShip() {
        return (standingShip != nullptr ? standingShip : nullptr);
    }
    
    friend class Board;
};
