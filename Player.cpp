//
//  Player.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include <iostream>

#include "Player.h"
#include "PlayerMoves.h"

using namespace std;

#pragma mark - Ctor

Player::Player(std::string name) : playerName(name) {
	aliveIns++;
}

Player::~Player() {
	aliveIns--;
}

int Player::aliveIns = 0;

#pragma mark - Public functions

void Player::updateName() {
    cout << "Please enter player name (old name is " << playerName << "):" << endl;
    cin >> playerName;
}

std::string Player::name() {
    return playerName;
}

void Player::resetScore() {
    playerScore = 0;
}

void Player::incrementScore(int byPoints)
{
    playerScore += byPoints;
}

int Player::score() {
    return playerScore;
}

void Player::setKeys(const char* keys)
{
	int i = 0;
	for (char& key : controlKeys) {
		key = keys[i++];
	}
}

void Player::notifyKeyHit(char ch, unsigned long ts)
{
	// Ignore if auto mode
	if (autoMode) { return; }

	// TODO: Consider using map / dictionary instead ?
	if (tolower(ch) == controlKeys[_SHIP1]) {
		setActiveShip(ships[_SHIP1]);
	}
	else if (tolower(ch) == controlKeys[_SHIP2]) {
		setActiveShip(ships[_SHIP2]);
	}
	else if (tolower(ch) == controlKeys[_SHIP3]) {
		setActiveShip(ships[_SHIP3]);
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

void Player::handleLoadedMoveIfNeeded(unsigned long ts) {
	if (!autoMode) { return; }

	const PlayerMoves::Move * const turn = moves().getMove(ts);
	if (turn) {
		int shipIndex = shipIndexByType((ShipType)turn->shipType());
		setActiveShip(ships[shipIndex]);
		Direction d = turn->direction();
		setActiveShipDirection(d, ts);
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
	setActiveShip(nullptr);
	for (int i = 0; i <= FLEET_SIZE; ++i) {
		ships[i]->resetToInitialState();
	}
}

void Player::clearFleetData() {
	setActiveShip(nullptr);
	for (int i = 0; i <= FLEET_SIZE; ++i) {
		delete ships[i];
		ships[i] = nullptr;
	}
}

#pragma mark - Private functions

void Player::setActiveShip(Ship* active) {
	activeShip = active;
}

void Player::setActiveShipDirection(Direction direction, unsigned long ts) {
	if (activeShip != nullptr) {
		activeShip->setDirection(direction);
		
		//Save move if not auto
		if (!autoMode) {
			moves().addMove(ts, *activeShip, direction);
		}
	}
}

int Player::shipIndexByType(ShipType type) {
	return ((type - 1) % FLEET_SIZE);
}

PlayerMoves & Player::moves() {
	// Lazy init
	if (movesMap == nullptr) {
		movesMap = new PlayerMoves();
		autoMode = false;
	}
	return *movesMap;
}

void Player::setMoves(PlayerMoves * moves) {
	movesMap = moves;
	autoMode = true;
}

bool Player::didFinishAutoMoves(unsigned long ts) {
	return autoMode && moves().isEnded(ts);
}
