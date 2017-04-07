//
//  Ship.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Ship.h"
#include "Cell.h"

#pragma mark - Ctor

Ship::Ship(ShipType type, Cell* startingCell) : shipType(type), initialCell(startingCell), currentCell(startingCell) { }

#pragma mark - Getters

Cell* Ship::cell() {
    return currentCell;
}

bool Ship::alive() {
    return isAlive;
}

#pragma mark - 

ShipType Ship::getShipType() {
    return shipType;
}

bool Ship::canMoveToCell(Cell& cell) {
    /// TODO: Amir: Implement Switch case
    return false;
}

void Ship::resetToInitialState() {
    // Set current cell's ship to nil
    currentCell->setStandingShip(nullptr);
    
    // Set initial cell's ship to this
    currentCell = initialCell;
    currentCell->setStandingShip(this);
}

bool Ship::didFindFlag() {
    /// TODO: Make sure we only look for oponent flag
    return (currentCell->getCellType() == CellType::FLAG_A ||
            currentCell->getCellType() == CellType::FLAG_B);
}
