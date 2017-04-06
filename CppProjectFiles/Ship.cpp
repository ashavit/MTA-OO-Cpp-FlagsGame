//
//  Ship.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Ship.h"

#pragma mark - Ctor

Ship::Ship(ShipType type, Cell& startingCell) : shipType(type), initialCell(&startingCell), currentCell(&startingCell) { }

#pragma mark - Getters

Cell* Ship::cell() {
    return currentCell;
}

bool Ship::alive() {
    return isAlive;
}

#pragma mark - 

bool Ship::canMoveToCell(Cell& cell) {
    /// TODO: Amir: Implement Switch case
    return false;
}

void Ship::resetToInitialState() {
    /// TODO: Amir: Set current cell's ship to nil
    
    /// TODO: Amir: Set initial cell's ship to this
    this->currentCell = this->initialCell;
    
}

Ship::ShipType Ship::getShipType() {
    return shipType;
}
