#include "BoardDataImpl.h"
#include "Board.h"

int BoardDataImpl::aliveIns = 0;

BoardDataImpl::BoardDataImpl(Board* board, int player) 
	: _board(board), _player(player) {
	++aliveIns;
}

BoardDataImpl::~BoardDataImpl() {
	--aliveIns;
}

// Coordinates start from 1,1
char BoardDataImpl::charAt(int x, int y) const {
	if (x >= 1 && x <= cols &&
		y >= 1 && y <= rows) {
		return _board->charAt(x-1, y-1);
	}
	return CellType::REGULAR;
}

