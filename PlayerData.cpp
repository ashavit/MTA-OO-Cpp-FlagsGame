#include <iostream>
#include "PlayerData.h"

using namespace std;

int PlayerData::aliveIns = 0;

PlayerData::PlayerData(std::string name) : playerName(name)
{
	++aliveIns;
}

PlayerData::~PlayerData()
{
	--aliveIns;
	delete playerImpl;
}

//*********** Public functions ***********//

void PlayerData::updateName() {
	cout << "Please enter player name (old name is " << playerName << "):" << endl;
	cin >> playerName;
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
