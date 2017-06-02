#include "FilePlayer.h"
#include "PlayerMoves.h"


FilePlayer::FilePlayer()
{
}


FilePlayer::~FilePlayer()
{
	clearMoves();
}

//*********** Private functions ***********//

PlayerMoves& FilePlayer::moves() {
	// Lazy init
	if (movesMap == nullptr) {
		movesMap = new PlayerMoves();
	}
	return *movesMap;
}

void FilePlayer::clearMoves() {
	if (movesMap) {
		delete movesMap;
		movesMap = nullptr;
	}
}

//*********** Public functions ***********//

void FilePlayer::handleLoadedMoveIfNeeded(unsigned long ts) {
	const PlayerMoves::Move* const turn = moves().getMove(ts);
	if (turn) {
		ShipType shipType = static_cast<ShipType>(turn->shipType());
		setActiveShip(shipType, ts);
		Direction d = turn->direction();
		setActiveShipDirection(d, ts);
	}
}

bool FilePlayer::didFinishAutoMoves(unsigned long ts) {
	return moves().isEnded(ts);
}

