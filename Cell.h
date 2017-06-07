#pragma once

#include "Config.h"
#include "Ship.h"
#include "Commons.h"

enum CellType
{
	REGULAR = ' ',
	FORREST = BOARD_MARK_FOREST,
	SEA = BOARD_MARK_SEA,
	FLAG_A = BOARD_MARK_FLAG_A,
	FLAG_B = BOARD_MARK_FLAG_B
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
	Cell(UINT row, UINT column, CellType type = REGULAR);
	~Cell();

	static int aliveInstances() { return aliveIns; }

	void setCellType(CellType aType) { type = aType; }

	CellType getCellType() const { return type; }

	int getRow() const { return row + 1; }
	char getColumn() const { return column + 'A'; }
	char charRepresentation() const;

	void setStandingShip(Ship* ship) { standingShip = ship; }

	Ship* getStandingShip() const {
		return (standingShip != nullptr ? standingShip : nullptr);
	}

	friend class Board;
};
