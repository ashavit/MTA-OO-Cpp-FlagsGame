#include <iostream>
#include "Player.h"
#include "PlayerMoves.h"

using namespace std;

//*********** Ctor ***********//

Player::Player(std::string name) : playerName(name) {
	aliveIns++;
}

Player::~Player() {
	aliveIns--;
}

int Player::aliveIns = 0;

//*********** Public functions ***********//

void Player::updateName() {
	cout << "Please enter player name (old name is " << playerName << "):" << endl;
	cin >> playerName;
}

std::string Player::name() const {
	return playerName;
}

void Player::resetScore() {
	playerScore = 0;
}

void Player::incrementScore(int byPoints) {
	playerScore += byPoints;
}

int Player::score() const {
	return playerScore;
}

void Player::setKeys(const char* keys) {
	int i = 0;
	for (char& key : controlKeys) {
		key = keys[i++];
	}
}

void Player::notifyKeyHit(char ch, unsigned long ts) {
	// Ignore if auto mode
	if (autoMode) { return; }

	// TODO: Consider using map / dictionary instead ?
	if (tolower(ch) == controlKeys[_SHIP1]) {
		setActiveShip(ships[_SHIP1]);
	}
	else if (tolower(ch) == controlKeys[_SHIP2]) {
		setActiveShip(ships[_SHIP2]);
	}
	else if (tolower(ch) == controlKeys[_SHIP3]) {
		setActiveShip(ships[_SHIP3]);
	}
	else if (tolower(ch) == controlKeys[_UPWARDS]) {
		setActiveShipDirection(Direction::UP, ts);
	}
	else if (tolower(ch) == controlKeys[_DOWN]) {
		setActiveShipDirection(Direction::DOWN, ts);
	}
	else if (tolower(ch) == controlKeys[_LEFT]) {
		setActiveShipDirection(Direction::LEFT, ts);
	}
	else if (tolower(ch) == controlKeys[_RIGHT]) {
		setActiveShipDirection(Direction::RIGHT, ts);
	}
	else if (tolower(ch) == controlKeys[_STOP]) {
		setActiveShipDirection(Direction::STOP, ts);
	}
}

void Player::handleLoadedMoveIfNeeded(unsigned long ts) {
	if (!autoMode) { return; }

	const PlayerMoves::Move* const turn = moves().getMove(ts);
	if (turn) {
		int shipIndex = shipIndexByType(static_cast<ShipType>(turn->shipType()));
		setActiveShip(ships[shipIndex]);
		Direction d = turn->direction();
		setActiveShipDirection(d, ts);
	}
}

bool Player::didPlayerWin() {
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr && ships[i]->alive() &&
			ships[i]->didFindFlag()) {
			return true;
		}
	}
	return false;
}

bool Player::didPlayerLose() {
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr && ships[i]->alive()) {
			return false;
		}
	}
	return true;
}

bool Player::addShip(Ship* ship) {
	int index = shipIndexByType(ship->type());
	if (ships[index] == nullptr) {
		ships[index] = ship;
		return true;
	}
	else {
		return false;
	}
}

void Player::restartGame() {
	setActiveShip(nullptr);
	for (int i = 0; i < FLEET_SIZE; ++i) {
		ships[i]->resetToInitialState();
	}

	// If not auto mode - reset all moves as well
	if (!autoMode) {
		delete movesMap;
		movesMap = nullptr;
	}
}

void Player::clearFleetData() {
	setActiveShip(nullptr);
	for (int i = 0; i < FLEET_SIZE; ++i) {
		if (ships[i] != nullptr)
			delete ships[i];
		ships[i] = nullptr;
	}
	delete movesMap;
	movesMap = nullptr;
}

//*********** Private functions ***********//

void Player::setActiveShip(Ship* active) {
	activeShip = active;
}

void Player::setActiveShipDirection(Direction direction, unsigned long ts) {
	if (activeShip != nullptr) {
		activeShip->setDirection(direction);

		//Save move if not auto
		if (!autoMode) {
			moves().addMove(ts, *activeShip, direction);
		}
	}
}

int Player::shipIndexByType(ShipType type) const {
	return ((type - 1) % FLEET_SIZE);
}

PlayerMoves& Player::moves() {
	// Lazy init
	if (movesMap == nullptr) {
		movesMap = new PlayerMoves();
	}
	return *movesMap;
}

bool Player::didFinishAutoMoves(unsigned long ts) {
	return autoMode && moves().isEnded(ts);
}

void Player::endMoveList(unsigned long ts) {
	setActiveShipDirection(Direction::STOP, ts);
}
