#pragma once

#include <string>
#include "Ship.h"

#define FLEET_SIZE 3

class PlayerMoves;

class Player
{
	static int aliveIns;

	std::string playerName;
	Ship* ships[FLEET_SIZE] = {nullptr};
	Ship* activeShip = nullptr;

	enum keyOptions
	{
		_SHIP1,
		_SHIP2,
		_SHIP3,
		_UPWARDS,
		_DOWN,
		_LEFT,
		_RIGHT,
		_STOP,
		_keyOptionsSize
	};

	char controlKeys[_keyOptionsSize];

	void setActiveShip(Ship* active, unsigned long timeStamp);
	void setActiveShipDirection(Direction direction, unsigned long timeStamp);
	int shipIndexByType(ShipType type) const;

	bool autoMode = false;
	PlayerMoves* movesMap = nullptr;

public:
	Player();
	~Player();
	Player(Player const&) = delete;
	void operator=(Player const&) = delete;
	static int aliveInstances() { return aliveIns; }

	virtual std::string getName() const { return playerName; }
	void setName(std::string name) { playerName = name; }

	void setKeys(const char* keys);
	void notifyKeyHit(char ch, unsigned long timeStamp);
	void handleLoadedMoveIfNeeded(unsigned long timeStamp);

	PlayerMoves& moves(); // Lazy init + convert pointer to reff
	void setMoves(PlayerMoves* moves) { movesMap = moves; };
	bool didFinishAutoMoves(unsigned long timeStamp);
	void setAutoMode(bool isAuto) { autoMode = isAuto; }
	bool isAutoMode() const { return autoMode; };
	void endMoveList(unsigned long timeStamp);

	bool didPlayerWin();
	bool didPlayerLose();

	bool addShip(Ship* ship);
	Ship* getShip(int index) { return ships[index]; }
	void restartGame();
	void clearFleetData();

};
