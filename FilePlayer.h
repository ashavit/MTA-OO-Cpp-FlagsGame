#pragma once

#include "Player.h"

class FilePlayer : public Player
{
	PlayerMoves* movesMap = nullptr;

	void clearMoves();

public:
	FilePlayer();
	virtual ~FilePlayer();
	FilePlayer(FilePlayer const&) = delete;
	void operator=(FilePlayer const&) = delete;

	PlayerMoves& moves(); // Lazy init + convert pointer to reff
	void setMoves(PlayerMoves* moves) { movesMap = moves; };
	void handleLoadedMoveIfNeeded(unsigned long timeStamp);
	bool didFinishAutoMoves(unsigned long timeStamp);

};

