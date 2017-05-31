#pragma once
#include "Game.h"

class Player;
class Flags;

class KeyboardGame : public Game
{
	static int aliveIns;

	bool _isRecordMode = false;

	virtual bool loadBoard(const std::string& fileName) override;
	virtual void drawCellIfNeeded(Cell *cell) const override { drawCell(cell);  };
	virtual void printBattleResultIfNeeded(std::string result) const override { printBattleResult(result); };
	virtual void handleKeyboardInput() override;
	virtual std::string endGameMessage() const override;
	virtual void delayEndGame() const override;
	virtual void postGameActions() const override;
	virtual void unpauseGame() const override { drawBoard(); }
	virtual void restartGame() override;

public:
	KeyboardGame(Player* playerA, Player* playerB, Flags* manager, int delay);
	KeyboardGame(KeyboardGame const&) = delete;
	void operator=(KeyboardGame const&) = delete;
	virtual ~KeyboardGame() override;

	virtual void setRecordMode(bool isRecordMode) override;
};

