#pragma once

#include <stdio.h>

class Cell;

enum ShipType {SHIP1 = 1, SHIP2, SHIP3, SHIP7 = 7, SHIP8, SHIP9 };

class Ship {
    
    const ShipType shipType;
    Cell* initialCell;
    Cell* currentCell;
    bool isAlive = true;
    
public:
    Ship(ShipType type, Cell* startingCell);
    
    Cell* cell();
    bool alive();
    ShipType getShipType();

    bool canMoveToCell(Cell& cell);
    void resetToInitialState();

    bool didFindFlag();
};
