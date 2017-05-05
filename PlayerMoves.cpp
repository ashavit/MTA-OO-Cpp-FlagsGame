#include "PlayerMoves.h"

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

void PlayerMoves::addMove(long ts, int ship, char dir)
{
	_moves.push_back(Move(ts, ship, dir));
}
