//
//  Ship.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Player.h"
#include "Ship.h"
#include "Cell.h"

#pragma mark - Ctor

Ship::Ship(Player& player, ShipType type, Cell* startingCell)
    : shipOwner(player), shipType(type), initialCell(startingCell), currentCell(startingCell)
{
    shipOwner.addShip(this);
}

#pragma mark - Getters

Cell* Ship::cell() {
    return currentCell;
}

bool Ship::alive() {
    return isAlive;
}

const Player& Ship::owner() {
    return shipOwner;
}

#pragma mark - 

ShipType Ship::type() {
    return shipType;
}

Direction Ship::direction() {
    return shipDirection;
}

void Ship::setDirection(Direction d) {
    shipDirection = d;
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
