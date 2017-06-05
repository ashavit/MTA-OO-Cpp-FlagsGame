#pragma once

#include "AbstractPlayer.h"
#include <map>

class AmirShavitPlayer : public AbstractPlayer
{
	static int aliveIns;

	enum Direction { STOP, UP, DOWN, LEFT, RIGHT, INVALID };

	int playerType = 0;

	const BoardData* boardData = nullptr;
	std::map<char, GameMove> myShips;
	std::map<char, GameMove> oponentShips;
//	GameMove myFlag;
//	GameMove oponentFlag;

	void resetPlayerState();
	char selectRandomActiveShip();
	GameMove selectRandomGameMove(char activeShip);

public:
	AmirShavitPlayer();
	virtual ~AmirShavitPlayer();
	AmirShavitPlayer(AmirShavitPlayer const&) = delete;
	void operator=(AmirShavitPlayer const&) = delete;

	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	virtual void setPlayer(int player) override;
	virtual void init(const BoardData& board) override;

	/* Coordinates start from 1,1 */
	virtual GameMove play(const GameMove& opponentsMove) override;
	virtual std::string getName() const override;

	static int aliveInstances() { return aliveIns; }
};

