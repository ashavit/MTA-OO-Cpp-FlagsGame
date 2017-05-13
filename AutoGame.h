#pragma once
#include "Game.h"

class Player;
class Flags;

class AutoGame : public Game
{
	static int aliveIns;

	bool isRecordMode = false;

	virtual bool loadBoard(const std::string& fileName) override;
	virtual void handleKeyboardInput() override;
	virtual std::string endGameMessage() const override;
	virtual void postGameActions() const override { }

	virtual bool isGameOver() const override;

public:
	AutoGame(Player& playerA, Player& playerB, Flags* manager, int delay);
	AutoGame(AutoGame const&) = delete;
	void operator=(AutoGame const&) = delete;
	virtual ~AutoGame() override;

	virtual void setRecordMode(bool isRecordMode) override {};
};

