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

void KeyboardPlayer::notifyKeyHit(char ch, unsigned long ts) {
	if (tolower(ch) == controlKeys[_SHIP1]) {
		setActiveShip(getShip(_SHIP1), ts);
	}
	else if (tolower(ch) == controlKeys[_SHIP2]) {
		setActiveShip(getShip(_SHIP2), ts);
	}
	else if (tolower(ch) == controlKeys[_SHIP3]) {
		setActiveShip(getShip(_SHIP3), ts);
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

