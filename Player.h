#pragma once

#include <string>
#include "AbstractPlayer.h"
#include "Ship.h"

#define FLEET_SIZE 3

class PlayerMoves;

class Player : public AbstractPlayer
{
	static int aliveIns;

	std::string playerName;
	int playerType = 0;
	Ship* ships[FLEET_SIZE] = {nullptr};
	Ship* activeShip = nullptr;

protected:
	int shipIndexByType(ShipType type) const;
	Ship* getActiveShip() const { return activeShip; }
	void setActiveShip(Ship* active, unsigned long timeStamp);
	virtual void setActiveShipDirection(Direction direction, unsigned long timeStamp);

public:
	Player();
	virtual ~Player();
	Player(Player const&) = delete;
	void operator=(Player const&) = delete;
	static int aliveInstances() { return aliveIns; }

	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	virtual void setPlayer(int player) override;
	int getPlayerType() const { return playerType; }

	virtual void init(const BoardData& board) override;

	virtual std::string getName() const override { return playerName; }
	void setName(std::string name) { playerName = name; }

	void endMoveList(unsigned long timeStamp);

	bool didPlayerWin();
	bool didPlayerLose();

	bool addShip(Ship* ship);
	Ship* getShip(int index) { return ships[index]; }
	void restartGame();
	void clearFleetData();

};
