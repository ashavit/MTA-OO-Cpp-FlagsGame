#pragma once

#include "AbstractPlayer.h"
#include <map>

struct Location
{
private:
	int _x, _y;

public:
	Location(int x = 0, int y = 0) : _x(x), _y(y) {}
	int x() { return _x; }
	int y() { return _y; }
};

class AmirShavitPlayer : public AbstractPlayer
{
	static int aliveIns;

	enum Direction { STOP, UP, DOWN, LEFT, RIGHT, INVALID };
	enum LocationType
	{
		REGULAR = ' ',
		FORREST = 'T',
		SEA = 'S'
	};

	int playerType = 0;

	const BoardData* boardData = nullptr;
	/* 0 indexes will be empty and ignored, to fit index to charAt function */
	char boardMap[BoardData::cols + 1][BoardData::rows + 1];
	std::map<char, GameMove> myShips;
	std::map<char, GameMove> oponentShips;
	Location myFlag;
	Location oponentFlag;

	void resetPlayerState();
	char selectRandomActiveShip();
	GameMove selectRandomGameMove(char activeShip);
	bool isMoveAllowed(char active_ship, Direction direction, int from_x, int from_y);

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

