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

void Player::init(const BoardData& bd) {
	resetPlayerState();
	boardData = &bd;
	for (UINT col = 1; col <= BoardData::cols; ++col) {
		for (UINT row = 1; row <= BoardData::rows; ++row) {
			char c = bd.charAt(col, row);
			if ((playerType == 1 && c >= '1' && c <= '3') ||
				(playerType == 2 && c >= '7' && c <= '9')) {
				ShipType type = static_cast<ShipType>(c - '0');
				ships.emplace(type, GameMove(col, row, col, row));
			}
		}
	}
}

GameMove Player::play(const GameMove& opponentsMove) {
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

//*********** Private functions ***********//

void Player::resetPlayerState() {
	ships.clear();
	activeShip = ShipType::INVALID;
	activeDirection = Direction::STOP;
	if (boardData)
		delete boardData;
	boardData = nullptr;
}

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
