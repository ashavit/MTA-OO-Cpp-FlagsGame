#pragma once

#include <iostream>
#include "Config.h"
#include "Ship.h"

enum CellType {REGULAR, FORREST, SEA, FLAG_A, FLAG_B};

class Cell {
	static int aliveIns;
    UINT   row ;
    UINT   column;
    CellType type;
    Ship*    standingShip = nullptr;

    void setCellType(CellType aType) { type = aType; }
    
public:
    Cell(uint16_t row, uint16_t column, CellType type = REGULAR)
        : row(row), column(column), type(type)  {
		aliveIns++;
	}

	~Cell() {
		aliveIns--;
	}

	static int aliveInstances() { return aliveIns; }

    CellType getCellType() const {
        return type;
    }

	int getRow() {
		return row + 1;
	}

	char getColumn() {
		return column + 'A';
	}

    void setStandingShip(Ship* ship) {
        standingShip = ship;
    }

    Ship* getStandingShip() {
        return (standingShip != nullptr ? standingShip : nullptr);
    }
    
    friend class Board;
};
