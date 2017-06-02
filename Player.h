#pragma once

#include <string>
#include <map>
#include "Commons.h"
#include "AbstractPlayer.h"
#include "Ship.h"

#define FLEET_SIZE 3

class PlayerMoves;

class Player : public AbstractPlayer
{
	static int aliveIns;

	std::string playerName;
	int playerType = 0;
	ShipType activeShip = (ShipType)0; // TODO: Replace force cast
	Direction activeDirection = Direction::STOP;

protected:

	std::map<ShipType, GameMove> ships;

	ShipType getActiveShip() const { return activeShip; }
	void setActiveShip(ShipType active, unsigned long timeStamp);
	void setActiveShipDirection(Direction direction, unsigned long timeStamp);

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
};
