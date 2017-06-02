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
			if (playerType == 1 && c >= '1' && c <= '3') {
				/// TODO: save ships ?
			}
			else if (playerType == 2 && c >= '7' && c <= '9') {

			}
		}
	}
}

bool Player::didPlayerWin() {
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr && ships[i]->alive() &&
			ships[i]->didFindFlag()) {
			return true;
		}
	}
	return false;
}

bool Player::didPlayerLose() {
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr && ships[i]->alive()) {
			return false;
		}
	}
	return true;
}

bool Player::addShip(Ship* ship) {
	int index = shipIndexByType(ship->type());
	if (ships[index] == nullptr) {
		ships[index] = ship;
		return true;
	}
	else {
		return false;
	}
}

void Player::restartGame() {
	setActiveShip(nullptr, 0);
	for (int i = 0; i < FLEET_SIZE; ++i) {
		ships[i]->resetToInitialState();
	}

	// TODO: move this to game to save to file - If not auto mode - reset all moves as well
//	if (!autoMode) {
//		delete movesMap;
//		movesMap = nullptr;
//	}
}

void Player::clearFleetData() {
	setActiveShip(nullptr,0);
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr)
			delete ships[i];
		ships[i] = nullptr;
	}
}

//*********** Private functions ***********//

void Player::setActiveShip(Ship* active, unsigned long ts) {
	if (activeShip != active) {
		setActiveShipDirection(Direction::STOP, ts);
		activeShip = active;
	}
}

void Player::setActiveShipDirection(Direction direction, unsigned long ts) {
	if (activeShip != nullptr)
		activeShip->setDirection(direction);
}

int Player::shipIndexByType(ShipType type) const {
	return ((type - 1) % FLEET_SIZE);
}

void Player::endMoveList(unsigned long ts) {
	setActiveShipDirection(Direction::STOP, ts);
}
