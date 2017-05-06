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

void Player::addShip(Ship* ship) {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        if (ships[i] == nullptr) {
            ships[i] = ship;
            return;
        }
    }
    
    cout << playerName << "can not add ship. Fleet is full" << endl;
}

void Player::addShipFromFile(Ship* ship) {
    int index = (ship->type() + 2) % 3;
    if (ships[index] == nullptr) {
        ships[index] = ship;
        return;
    }
    // else Don nothing. File loader will handle
}

void Player::restartGame() {
	setActiveShip(nullptr);
	for (int i = _SHIP1; i <= _SHIP3; ++i) {
		ships[i]->resetToInitialState();
	}
}

void Player::clearFleetData() {
	setActiveShip(nullptr);
	for (int i = _SHIP1; i <= _SHIP3; ++i) {
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

PlayerMoves & Player::moves()
{
	// Lazy init
	if (movesMap == nullptr) {
		movesMap = new PlayerMoves();
		autoMode = false;
	}
	return *movesMap;
}

void Player::setMoves(PlayerMoves * moves)
{
	movesMap = moves;
	autoMode = true;
}

bool Player::didFinishAutoMoves(unsigned long ts)
{
	return autoMode;// && moves().isEnded(ts);
}