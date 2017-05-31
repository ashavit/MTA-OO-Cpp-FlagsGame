#pragma once
#include "Game.h"

class Player;
class Flags;

class AutoGame : public Game
{
	static int aliveIns;

	bool isQuietMode = false;

	void drawBoardIfNeeded() const;

	virtual bool loadBoard(const std::string& fileName) override;
	virtual void drawCellIfNeeded(Cell *cell) const override;
	virtual void printBattleResultIfNeeded(std::string result) const override;
	virtual void handleKeyboardInput() override;
	virtual std::string endGameMessage() const override;
	virtual void delayEndGame() const override;
	virtual void postGameActions() const override { }
	virtual void unpauseGame() const override;

	virtual bool isGameOver() const override;

public:
	AutoGame(Player* playerA, Player* playerB, Flags* manager, int delay);
	AutoGame(AutoGame const&) = delete;
	void operator=(AutoGame const&) = delete;
	virtual ~AutoGame() override;

	virtual void setRecordMode(bool isRecordMode) override {};
	void setQuietMode(bool isQuiet) { isQuietMode = isQuiet; }
};
