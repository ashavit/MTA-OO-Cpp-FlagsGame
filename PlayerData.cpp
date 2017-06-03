#include <iostream>
#include "PlayerData.h"
#include "KeyboardPlayer.h"
#include "FilePlayer.h"

using namespace std;

int PlayerData::aliveIns = 0;

PlayerData::PlayerData(std::string name) : playerName(name) {
	++aliveIns;
}

PlayerData::~PlayerData() {
	--aliveIns;
	delete playerImpl;
}

//*********** Public functions ***********//

void PlayerData::initPlayer(ConfigurationManager::MovesMode mode) {
	if (mode == ConfigurationManager::MovesMode::MOVES_ALGO) {
		
	}
	else if (mode == ConfigurationManager::MovesMode::MOVES_FILE) {
		FilePlayer* fPlayer = new FilePlayer();
		fPlayer->setName(playerName);
		playerImpl = fPlayer;
	}
	else {
		KeyboardPlayer* kPlayer = new KeyboardPlayer();
		kPlayer->setName(playerName);
		playerImpl = kPlayer;
	}
}

void PlayerData::freePlayer() {
	if (playerImpl)
		delete playerImpl;
	playerImpl = nullptr;
}

void PlayerData::updateName() {
	cout << "Please enter player name (old name is " << playerName << "):" << endl;
	cin >> playerName;

	Player* pPlayer = dynamic_cast<Player*>(playerImpl);
	if (pPlayer)
		pPlayer->setName(playerName);
}

std::string PlayerData::name() const {
	return playerName;
}

void PlayerData::resetScore() {
	playerScore = 0;
}

void PlayerData::incrementScore(int byPoints) {
	playerScore += byPoints;
}

int PlayerData::score() const {
	return playerScore;
}
