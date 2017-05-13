#pragma once

#include "Config.h"
#include "Ship.h"

enum CellType
{
	REGULAR,
	FORREST,
	SEA,
	FLAG_A,
	FLAG_B
};

class Cell
{
	Cell(Cell const&) = delete;
	void operator=(Cell const&) = delete;

	static int aliveIns;
	const UINT row;
	const UINT column;
	CellType type;
	Ship* standingShip = nullptr;

public:
	Cell(UINT row, UINT column, CellType type = REGULAR)
		: row(row), column(column), type(type) {
		aliveIns++;
	}

	~Cell() {
		aliveIns--;
	}

	static int aliveInstances() { return aliveIns; }

	void setCellType(CellType aType) { type = aType; }

	CellType getCellType() const { return type; }

	int getRow() const { return row + 1; }

	char getColumn() const { return column + 'A'; }

	void setStandingShip(Ship* ship) { standingShip = ship; }

	Ship* getStandingShip() const {
		return (standingShip != nullptr ? standingShip : nullptr);
	}

	friend class Board;
};
