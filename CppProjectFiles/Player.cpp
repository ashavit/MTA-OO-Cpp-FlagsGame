//
//  Player.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Player.h"

Player::Player(std::string name):playerName(name) {
}

std::string Player::name() {
    return playerName;
}

int Player::score() {
    return playerScore;
}
