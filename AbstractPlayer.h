#pragma once

#include <string>
#include "GameMove.h"
#include "BoardData.h"

class AbstractPlayer {
public:
	virtual ~AbstractPlayer() { }
	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	virtual void setPlayer(int player) = 0;
	virtual void init(const BoardData& board) = 0;
	/* Coordinates start from 1,1 */
	virtual GameMove play(const GameMove& opponentsMove) = 0;
	virtual std::string getName() const = 0;
};