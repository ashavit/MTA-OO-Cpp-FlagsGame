#pragma once

#include <string>
#include "AbstractPlayer.h"
#include "Player.h"
#include "ConfigurationManager.h"

class PlayerData
{
	static int aliveIns;

	Player *playerImpl = nullptr;
	std::string playerName;
	int playerScore = 0;

public:
	PlayerData(std::string name);
	~PlayerData();
	PlayerData(PlayerData const&) = delete;
	void operator=(PlayerData const&) = delete;

	void initPlayer(ConfigurationManager::MovesMode mode);
	void freePlayer();

	void updateName();
	std::string name() const;

	void resetScore();
	void incrementScore(int byPoints);
	int score() const;

	Player* player() const { return playerImpl; }

	bool operator==(const PlayerData& other) const {
		return (playerName == other.playerName);
	}

	bool operator!=(const PlayerData& other) const {
		return (playerName != other.playerName);
	}

	static int aliveInstances() { return aliveIns; }
};

