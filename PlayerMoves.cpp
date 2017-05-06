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

const PlayerMoves::Move *const  PlayerMoves::getMove(unsigned long ts) {
	PlayerMoves::Move* res = nullptr;
	auto itr = _moves.find(ts);
	if (itr != _moves.end())
		res = itr->second;
	return res;
}

bool PlayerMoves::isEnded(unsigned long ts)
{
	return (ts > _moves.crbegin()->first);
}

Direction PlayerMoves::Move::direction() const
{
	switch (_direction) {
	case 'U':
	case 'u':
		return Direction::UP;
	case 'D':
	case 'd':
		return Direction::DOWN;
	case 'L':
	case 'l':
		return Direction::LEFT;
	case 'R':
	case 'r':
		return Direction::RIGHT;
	case 'S':
	case 's':
	default:
		return Direction::STOP;
	}
}
