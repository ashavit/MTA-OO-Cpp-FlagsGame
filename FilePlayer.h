#pragma once

#include "Player.h"

class FilePlayer : public Player
{
	PlayerMoves* movesMap = nullptr;
	unsigned long lastTurnTimeStamp = 0;
	void clearMoves();

public:
	FilePlayer();
	virtual ~FilePlayer();
	FilePlayer(FilePlayer const&) = delete;
	void operator=(FilePlayer const&) = delete;

	void setCurrentTimeStamp(unsigned long timeStamp) { lastTurnTimeStamp = timeStamp; }
	virtual GameMove play(const GameMove& opponentsMove) override;

	PlayerMoves& moves(); // Lazy init + convert pointer to reff
	void setMoves(PlayerMoves* moves) { movesMap = moves; };
	void handleLoadedMoveIfNeeded(unsigned long timeStamp);
	bool didFinishAutoMoves(unsigned long timeStamp);

};

