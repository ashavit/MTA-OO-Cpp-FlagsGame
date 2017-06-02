#include "KeyboardPlayer.h"

KeyboardPlayer::KeyboardPlayer()
{
}


KeyboardPlayer::~KeyboardPlayer()
{
}

void KeyboardPlayer::setKeys(const char* keys) {
	int i = 0;
	for (char& key : controlKeys) {
		key = keys[i++];
	}
}

//*********** Public functions ***********//

void KeyboardPlayer::setPlayer(int player) {
	Player::setPlayer(player);
	if (player == 1)
		setKeys("123wxads");
	else if (player == 2)
		setKeys("789imjlk");
}

GameMove KeyboardPlayer::play(const GameMove& opponentsMove) {
	// Check if ship is still alive
	auto search = ships.find(getActiveShip());
	if (search == ships.end())
		return GameMove(0, 0, 0, 0);

	GameMove shipLastMove = search->second;
	int fromX = shipLastMove.from_x;
	int fromY = shipLastMove.from_y;
	int toX = shipLastMove.to_x;
	int toY = shipLastMove.to_y;

	// Check if ship has moved - remove from map to reassign after move
	ships.erase(search);
	if (boardData->charAt(toX, toY) == ('0' + getActiveShip())) {
		fromX = toX;
		fromY = toY;
	}
	// Check if ship was stuck in last place - try to move again
	else if (boardData->charAt(fromX, fromY) == ('0' + getActiveShip())) {
		toX = fromX;
		toY = fromY;
	}
	else { // Ship is dead
		return GameMove(0, 0, 0, 0);
	}

	// Try to move
	if (getActiveShipDirection() == Direction::UP) { --toY; }
	else if (getActiveShipDirection() == Direction::DOWN) { ++toY; }
	else if (getActiveShipDirection() == Direction::LEFT) { --toX; }
	else if (getActiveShipDirection() == Direction::RIGHT) { ++toX; }

	GameMove res = GameMove(fromX, fromY, toX, toY);
	ships.emplace(getActiveShip(), res);
	return res;
}

void KeyboardPlayer::notifyKeyHit(char ch, unsigned long ts) {
	if (tolower(ch) == controlKeys[_SHIP1]) {
		ShipType newActive = (getPlayerType() == 1 ? ShipType::SHIP1 : ShipType::SHIP7);
		setActiveShip(newActive, ts);
	}
	else if (tolower(ch) == controlKeys[_SHIP2]) {
		ShipType newActive = (getPlayerType() == 1 ? ShipType::SHIP2 : ShipType::SHIP8);
		setActiveShip(newActive, ts);
	}
	else if (tolower(ch) == controlKeys[_SHIP3]) {
		ShipType newActive = (getPlayerType() == 1 ? ShipType::SHIP3 : ShipType::SHIP9);
		setActiveShip(newActive, ts);
	}
	else if (tolower(ch) == controlKeys[_UPWARDS]) {
		setActiveShipDirection(Direction::UP, ts);
	}
	else if (tolower(ch) == controlKeys[_DOWN]) {
		setActiveShipDirection(Direction::DOWN, ts);
	}
	else if (tolower(ch) == controlKeys[_LEFT]) {
		setActiveShipDirection(Direction::LEFT, ts);
	}
	else if (tolower(ch) == controlKeys[_RIGHT]) {
		setActiveShipDirection(Direction::RIGHT, ts);
	}
	else if (tolower(ch) == controlKeys[_STOP]) {
		setActiveShipDirection(Direction::STOP, ts);
	}
}

