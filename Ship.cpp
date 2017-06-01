#include "Ship.h"
#include "Cell.h"

//*********** Ctor ***********//

Ship::Ship(ShipType type, Cell* startingCell)
	: shipType(type), initialCell(startingCell), currentCell(startingCell) {
	aliveIns++;
}

Ship::~Ship() {
	aliveIns--;
}

int Ship::aliveIns = 0;

//*********** Getters ***********//

Cell* Ship::cell() const {
	return currentCell;
}

bool Ship::alive() const {
	return isAlive;
}

void Ship::setDead() {
	isAlive = false;
	setDirection(Direction::STOP);
	currentCell->setStandingShip(nullptr);
	currentCell = nullptr;
}

//*********** Other functions ***********//

ShipType Ship::type() const {
	return shipType;
}

Direction Ship::direction() const {
	return shipDirection;
}

void Ship::setDirection(Direction d) {
	shipDirection = d;
}

bool Ship::canMoveToCell(const Cell* cell) const {
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

bool Ship::didFindFlag() const {
	return (currentCell->getCellType() == CellType::FLAG_A ||
		currentCell->getCellType() == CellType::FLAG_B);
}
