#pragma once

class Player;
class Cell;

enum class Direction
{
	STOP,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	INVALID
};

enum ShipType
{
	SHIP1 = 1,
	SHIP2,
	SHIP3,
	SHIP7 = 7,
	SHIP8,
	SHIP9
};

class Ship
{
	static int aliveIns;

	Player* shipOwner;
	const ShipType shipType;
	Cell* initialCell;
	Cell* currentCell;
	bool isAlive = true;
	Direction shipDirection = Direction::STOP;

public:
	Ship(Player* player, ShipType type, Cell* startingCell);
	~Ship();
	Ship(Ship const&) = delete;
	void operator=(Ship const&) = delete;
	static int aliveInstances() { return aliveIns; }

	Cell* cell() const;
	bool alive() const;
	void setDead();
	const Player* owner() const;
	ShipType type() const;

	Direction direction() const;
	void setDirection(Direction d);

	bool canMoveToCell(const Cell* cell) const;
	void moveToCell(Cell* cell);
	void resetToInitialState();

	bool didFindFlag() const;
};
