#pragma once

#include "BoardData.h"

class Board;

class BoardDataImpl : public BoardData
{
	static int aliveIns;
	const Board* _board;
	/* player: 1 for 1-2-3 player, 2 for 7-8-9 */
	const int _player;

public:

	static int aliveInstances() { return aliveIns; }

	BoardDataImpl(Board* board, int player);
	virtual ~BoardDataImpl();

	// Coordinates start from 1,1
	virtual char charAt(int x, int y) const override;
	
};
