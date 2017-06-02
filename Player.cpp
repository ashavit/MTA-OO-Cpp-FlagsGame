#include <iostream>
#include "Config.h"
#include "Player.h"
#include "PlayerMoves.h"

using namespace std;

//*********** Ctor ***********//

Player::Player() {
	aliveIns++;
}

Player::~Player() {
	aliveIns--;
}

int Player::aliveIns = 0;

//*********** Public functions ***********//

void Player::setPlayer(int player) {
	if (player == 1 || player == 2)
		playerType = player;
}

void Player::init(const BoardData& board) {
	for (UINT col = 1; col < BoardData::cols; ++col) {
		for (UINT row = 1; row < BoardData::rows; ++row) {
			char c = board.charAt(col, row);
			if ((playerType == 1 && c >= '1' && c <= '3') ||
				(playerType == 2 && c >= '7' && c <= '9')) {
				ShipType type = static_cast<ShipType>(c - '0');
				ships[type] = TurnMove(col, row, col, row);
			}
		}
	}
}

bool Player::didPlayerWin() {
	return false;
}

bool Player::didPlayerLose() {
	return false;
}

//*********** Private functions ***********//

void Player::setActiveShip(ShipType active, unsigned long ts) {
	if (activeShip != active) {
		setActiveShipDirection(Direction::STOP, ts);
		activeShip = active;
	}
}

void Player::setActiveShipDirection(Direction direction, unsigned long ts) {
	activeDirection = direction;
}

void Player::endMoveList(unsigned long ts) {
	setActiveShipDirection(Direction::STOP, ts);
}
