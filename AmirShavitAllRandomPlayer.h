#pragma once

#include "AbstractPlayer.h"
#include <map>

class AmirShavitAllRandomPlayer : public AbstractPlayer
{
	static int aliveIns;

	enum Direction { STOP, UP, DOWN, LEFT, RIGHT, INVALID };

	int playerType = 0;

	const BoardData* boardData = nullptr;
	std::map<char, GameMove> myShips;
	std::map<char, GameMove> oponentShips;

	void resetPlayerState();
	char selectRandomActiveShip();
	GameMove selectRandomGameMove(char activeShip);

public:
	AmirShavitAllRandomPlayer();
	virtual ~AmirShavitAllRandomPlayer();
	AmirShavitAllRandomPlayer(AmirShavitAllRandomPlayer const&) = delete;
	void operator=(AmirShavitAllRandomPlayer const&) = delete;

	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	virtual void setPlayer(int player) override;
	virtual void init(const BoardData& board) override;

	/* Coordinates start from 1,1 */
	virtual GameMove play(const GameMove& opponentsMove) override;
	virtual std::string getName() const override;

	static int aliveInstances() { return aliveIns; }
};

