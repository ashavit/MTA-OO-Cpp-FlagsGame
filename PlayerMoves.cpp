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
	for (auto& turn : _moves) {
		delete turn.second;
	}
	aliveIns--;
}

void PlayerMoves::addMove(unsigned long ts, int ship, char dir)
{
	_moves[ts] = new Move(ship, dir);
}

void PlayerMoves::addMove(unsigned long ts, Ship & ship, Direction dir)
{
	addMove(ts, (int)ship.type(), PlayerMoves::charFromDirection(dir));
}

const PlayerMoves::Move *const  PlayerMoves::getMove(unsigned long ts) {
	PlayerMoves::Move* res = nullptr;
	auto itr = _moves.find(ts);
	if (itr != _moves.end() && itr->second->isMoveValid()) // Ignore bad moves
		res = itr->second;

	return res;
}

bool PlayerMoves::isEnded(unsigned long ts)
{
	return (ts > _moves.crbegin()->first);
}

Direction PlayerMoves::directionFromChar(char _direction) {
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
		return Direction::STOP;
	default:
		return Direction::INVALID;
	}
}

char PlayerMoves::charFromDirection(Direction _direction) {
	switch (_direction) {
	case Direction::UP:
		return 'U';
	case Direction::DOWN:
		return 'D';
	case Direction::LEFT:
		return 'L';
	case Direction::RIGHT:
		return 'R';
	case Direction::STOP:
		return 'S';
	default:
		return ' ';
	}
}

Direction PlayerMoves::Move::direction() const {
	return PlayerMoves::directionFromChar(_direction);
}

bool PlayerMoves::Move::isMoveValid() {
	return ((PlayerMoves::directionFromChar(_direction) != Direction::INVALID) &&
		(_shipType == ShipType::SHIP1 || _shipType == ShipType::SHIP2 ||
			_shipType == ShipType::SHIP3 || _shipType == ShipType::SHIP7 ||
			_shipType == ShipType::SHIP8 || _shipType == ShipType::SHIP9));
}

std::ostream& operator<<(std::ostream& out, const PlayerMoves::Move& move) {
	return out << move._shipType << ", " << move._direction;
}

