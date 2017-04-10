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
	aliveIns++;
    shipOwner.addShip(this);
}

Ship::~Ship() {
	aliveIns--;
}

int Ship::aliveIns = 0;

#pragma mark - Getters

Cell* Ship::cell() {
    return currentCell;
}

bool Ship::alive() {
    return isAlive;
}

void Ship::setDead() {
	isAlive = false;
	setDirection(Direction::STOP);
	currentCell->setStandingShip(nullptr);
	currentCell = nullptr;
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

bool Ship::canMoveToCell(const Cell* cell) {
    if (cell->getCellType() == CellType::FORREST) {
        return (shipType == SHIP2 || shipType == SHIP7 || shipType == SHIP8);
    }
    else if (cell->getCellType() == CellType::SEA) {
        return (shipType == SHIP2 || shipType == SHIP3 || shipType == SHIP7);
    }
	else if (cell->getCellType() == CellType::FLAG_A) {
		return (shipType == SHIP7 || shipType == SHIP8 || shipType == SHIP9);
	}
	else if (cell->getCellType() == CellType::FLAG_B) {
		return (shipType == SHIP1 || shipType == SHIP2 || shipType == SHIP3);
	}
	return true;
}

void Ship::moveToCell(Cell* cell) {
    // Set current cell's ship to nil
    currentCell->setStandingShip(nullptr);
    
    // Set initial cell's ship to this
    currentCell = cell;
    currentCell->setStandingShip(this);
}

void Ship::resetToInitialState() {
	isAlive = true;
	setDirection(Direction::STOP);
	
	// Set current cell's ship to nil
    currentCell->setStandingShip(nullptr);
    
    // Set initial cell's ship to this
    currentCell = initialCell;
    currentCell->setStandingShip(this);
}

bool Ship::didFindFlag() {
    return (currentCell->getCellType() == CellType::FLAG_A ||
            currentCell->getCellType() == CellType::FLAG_B);
}
