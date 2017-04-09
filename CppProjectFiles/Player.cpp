//
//  Player.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include <iostream>

#include "Player.h"

using namespace std;

#pragma mark - Ctor

Player::Player(std::string name) : playerName(name) {
}

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

void Player::notifyKeyHit(char ch)
{
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
		activeShip->setDirection(Direction::UP);
	}
	else if (tolower(ch) == controlKeys[_DOWN]) {
		activeShip->setDirection(Direction::DOWN);
	}
	else if (tolower(ch) == controlKeys[_LEFT]) {
		activeShip->setDirection(Direction::LEFT);
	}
	else if (tolower(ch) == controlKeys[_RIGHT]) {
		activeShip->setDirection(Direction::RIGHT);
	}
	else if (tolower(ch) == controlKeys[_STOP]) {
		activeShip->setDirection(Direction::STOP);
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

bool Player::didPlayerLoose() {
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

Ship* Player::getFleet() {
    return nullptr;
}

#pragma mark - Private functions

void Player::setActiveShip(Ship* active) {
	activeShip = active;
}