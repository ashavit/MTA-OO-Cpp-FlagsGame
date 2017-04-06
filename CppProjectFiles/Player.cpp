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

Ship* Player::getFleet() {
    return nullptr;
}

#pragma mark - Private functions

