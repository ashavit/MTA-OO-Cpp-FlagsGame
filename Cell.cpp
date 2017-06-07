#include "Cell.h"

int Cell::aliveIns = 0;

Cell::Cell(UINT row, UINT column, CellType type) : row(row), column(column), type(type) {
	++aliveIns;
}

Cell::~Cell() {
	--aliveIns;
}

char Cell::charRepresentation() const {
	if (getStandingShip() != nullptr && getStandingShip()->alive()) {
		int type = getStandingShip()->type();
		return '0' + type;
	} else {
		return getCellType();
	}
}
