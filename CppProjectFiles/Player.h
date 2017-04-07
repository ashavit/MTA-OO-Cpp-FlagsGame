#pragma once

#include <stdio.h>
#include <string>
#include "Ship.h"

#define FLEET_SIZE 3

class Player {
    std::string playerName;
    int playerScore = 0;
    Ship* ships[FLEET_SIZE] = { nullptr };
    Ship* activeShip = nullptr;
    
public:
    Player(std::string name);
    
    void updateName();
    std::string name();

    void resetScore();
    void incrementScore(int byPoints);
    int score();
    
    bool didPlayerWin();
    bool didPlayerLoose();
    
    void addShip(Ship* ship);
    Ship* getFleet();
    
};
