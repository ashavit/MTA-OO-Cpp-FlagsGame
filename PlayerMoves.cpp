#include "PlayerMoves.h"
#include "Ship.h"

int PlayerMoves::aliveIns = 0;
int PlayerMoves::Move::aliveIns = 0;

PlayerMoves::PlayerMoves()
{
	aliveIns++;
}


PlayerMoves::~PlayerMoves()
{
	aliveIns--;
}

void PlayerMoves::addMove(unsigned long ts, int ship, char dir)
{
	_moves[ts] = new Move(ship, dir);
}

void PlayerMoves::addMove(unsigned long ts, Ship & ship, Direction dir)
{
	addMove(ts, (int)ship.type(), 0);
}

bool PlayerMoves::isEnded(unsigned long ts)
{
	return (ts > _moves.crbegin()->first);
}
