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
	struct TurnMove
	{
		int from_x, from_y;
		int to_x, to_y;
		TurnMove() : TurnMove(-1, -1, -1, -1) { }
		TurnMove(int x1, int y1, int x2, int y2) : from_x(x1), from_y(y1), to_x(x2), to_y(y2) { }
		friend Player;
	};

	std::map<ShipType, TurnMove> ships;

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
