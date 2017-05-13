#pragma once
#include "Game.h"

class Player;
class Flags;

class KeyboardGame : public Game
{
	static int aliveIns;

	bool isRecordMode = false;

	virtual bool loadBoard(const std::string& fileName) override;
	virtual void handleKeyboardInput() override;
	virtual std::string endGameMessage() const override;
	virtual void delayEndGame() const override;
	virtual void postGameActions() const override;

	virtual void restartGame() override;

public:
	KeyboardGame(Player& playerA, Player& playerB, Flags* manager, int delay);
	KeyboardGame(KeyboardGame const&) = delete;
	void operator=(KeyboardGame const&) = delete;
	virtual ~KeyboardGame() override;

	virtual void setRecordMode(bool isRecordMode) override;
};

