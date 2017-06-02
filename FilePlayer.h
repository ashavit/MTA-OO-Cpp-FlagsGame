#pragma once

#include "Player.h"

class FilePlayer : public Player
{
	PlayerMoves* movesMap = nullptr;

	void clearMoves();
	virtual void setActiveShipDirection(Direction direction, unsigned long timeStamp) override;

public:
	FilePlayer();
	~FilePlayer();

	PlayerMoves& moves(); // Lazy init + convert pointer to reff
	void setMoves(PlayerMoves* moves) { movesMap = moves; };
	void handleLoadedMoveIfNeeded(unsigned long timeStamp);
	bool didFinishAutoMoves(unsigned long timeStamp);

};

