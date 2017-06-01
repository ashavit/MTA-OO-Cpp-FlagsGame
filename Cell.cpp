#include "Cell.h"

int Cell::aliveIns = 0;

char Cell::charRepresentation() const {
	if (getStandingShip() != nullptr && getStandingShip()->alive()) {
		int type = getStandingShip()->type();
		return '0' + type;
	} else {
		return getCellType();
	}
}
