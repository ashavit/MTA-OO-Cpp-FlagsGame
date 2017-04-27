#pragma once

#include <stdio.h>
#include <string>
#include "Ship.h"

#define FLEET_SIZE 3

class Player {
	static int aliveIns;

    std::string playerName;
    int playerScore = 0;
    Ship* ships[FLEET_SIZE] = { nullptr };
    Ship* activeShip = nullptr;

	enum keyOptions { _SHIP1, _SHIP2, _SHIP3, _UPWARDS, _DOWN, _LEFT, _RIGHT, _STOP, _keyOptionsSize };
	char controlKeys[_keyOptionsSize];

	void setActiveShip(Ship* active);
    
public:
    Player(std::string name);
	~Player();
	static int aliveInstances() { return aliveIns; }

    void updateName();
    std::string name();

    void resetScore();
    void incrementScore(int byPoints);
    int score();
    
	void setKeys(const char* keys);
	void notifyKeyHit(char ch);

    bool didPlayerWin();
    bool didPlayerLose();
    
    void addShip(Ship* ship);
    void addShipFromFile(Ship* ship);
    Ship* getShip(int index) { return ships[index]; }
	void restartGame();
    void clearFleetData();
    
    bool operator== (const Player &other) const {
        return (playerName == other.playerName);
    }
    
    bool operator!= (const Player &other) const {
        return (playerName != other.playerName);
    }
    
};
