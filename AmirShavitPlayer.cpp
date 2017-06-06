#include "AmirShavitPlayer.h"
#include "AlgorithmRegistration.h"
#include <string>
#include <functional>

// TODO: Replace with ID
AlgorithmRegistration algo_AmirShavitStudent("algo_AmirShavitStudent", [] { return new AmirShavitPlayer(); });

int AmirShavitPlayer::aliveIns = 0;

AmirShavitPlayer::AmirShavitPlayer() {
	++aliveIns;
}


AmirShavitPlayer::~AmirShavitPlayer() {
	--aliveIns;
}

//*********** Private functions ***********//

void AmirShavitPlayer::resetPlayerState() {
	myShips.clear();
	oponentShips.clear();
//	activeShip = (ShipType)0; // TODO: Replace force cast
//	activeDirection = Direction::STOP;
	boardData = nullptr;
}

char AmirShavitPlayer::selectRandomActiveShip() {
	char activeShip = '0';
	do {
		int shipIndex = std::rand() % (myShips.size());
		auto shipItr = myShips.begin();
		for (int i = 0; i < shipIndex; ++i) { ++shipItr; }

		activeShip = shipItr->first;
		GameMove shipLastMove = shipItr->second;

		// Check if ship has moved - remove from map to reassign after move
		myShips.erase(shipItr);
		if (boardData->charAt(shipLastMove.to_x, shipLastMove.to_y) == activeShip) {
			myShips.emplace(activeShip, GameMove(shipLastMove.to_x, shipLastMove.to_y, shipLastMove.to_x, shipLastMove.to_y));
		}
		else if (boardData->charAt(shipLastMove.from_x, shipLastMove.from_y) == activeShip) {
			myShips.emplace(activeShip, GameMove(shipLastMove.from_x, shipLastMove.from_y, shipLastMove.from_x, shipLastMove.from_y));
		}
		else {
			activeShip = '0';
		}
	} while (activeShip < '1');
	return activeShip;
}

GameMove AmirShavitPlayer::selectRandomGameMove(char activeShip) {
	auto shipItr = myShips.find(activeShip);
	GameMove shipLastMove = shipItr->second;
	int fromX = shipLastMove.from_x;
	int fromY = shipLastMove.from_y;
	int toX = shipLastMove.to_x;
	int toY = shipLastMove.to_y;

	// Remove from map to reassign after move
	myShips.erase(shipItr);

	int directionIndex = 1 + std::rand() % 4;

	// Try to move
	if (directionIndex == Direction::UP) { --toY; }
	else if (directionIndex == Direction::DOWN) { ++toY; }
	else if (directionIndex == Direction::LEFT) { --toX; }
	else if (directionIndex == Direction::RIGHT) { ++toX; }
	return GameMove(fromX, fromY, toX, toY);
}

//*********** Public functions ***********//

void AmirShavitPlayer::setPlayer(int player) {
	if (player == 1 || player == 2)
		playerType = player;
}

void AmirShavitPlayer::init(const BoardData& board) {
	resetPlayerState();
	boardData = &board;
	for (unsigned int col = 1; col <= BoardData::cols; ++col) {
		for (unsigned int row = 1; row <= BoardData::rows; ++row) {
			boardMap[col][row] = LocationType::REGULAR;
			char c = board.charAt(col, row);
			if (c == LocationType::FORREST) {
				boardMap[col][row] = LocationType::FORREST;
			}
			else if (c == LocationType::SEA) {
				boardMap[col][row] = LocationType::SEA;
			}
			else if (c >= '1' && c <= '9') {
				myShips.emplace(c, GameMove(col, row, col, row));
			}
			else if (c == '#') {
				oponentShips.emplace(c, GameMove(col, row, col, row));
			}
			else if (c == 'A') {
				myFlag = Location(col, row);
			}
			else if (c == 'B') {
				oponentFlag = Location(col, row);
			}
		}
	}
}

GameMove AmirShavitPlayer::play(const GameMove& opponentsMove) {
	char activeShip = selectRandomActiveShip();
	GameMove res = selectRandomGameMove(activeShip);
	myShips.emplace(activeShip, res);
	return res;
}

std::string AmirShavitPlayer::getName() const {
	return "Amir Shavit's algo player";
}


