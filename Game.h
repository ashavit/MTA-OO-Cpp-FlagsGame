#pragma once

#include <stdio.h>
#include <string>

class Flags;
class Player;
class Ship;
class Board;
class Cell;

class Game {
	static int aliveIns;
    static int roundCounter;

	enum Awards { WIN = 50, LOSS = 30 };
	enum GameState { IN_PROGRESS, ABORTED, ABORT_AND_QUIT };

    Flags* gameManager = nullptr;
    
    GameState gameState = GameState::IN_PROGRESS;
	unsigned long _timeStamp = 0;

    void handlePlayerTurn(Player& p) const;
	void handleBattle(Ship* shipA, Ship* shipB, Cell* cell) const;
	void endGame() const;
    void awardWinner() const;
	void displaySubMenu();

protected:
	Player& playerA;
	Player& playerB;
	int delayTurnPeriod;
	Board* gameBoard = nullptr;
	std::string gameName;

	void drawBoard() const;
	void notifyKeyHit(char ch);
	unsigned long timeStamp() const { return _timeStamp; };

	virtual bool loadBoard(const std::string& fileName) = 0;
	virtual void handleKeyboardInput() = 0;
	virtual std::string endGameMessage() const = 0;
	virtual void postGameActions() const = 0;

	virtual bool isGameOver() const;
	virtual void restartGame();

public:
    Game(Player& playerA, Player& playerB, Flags* manager, int delay);
	virtual ~Game();
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	static int aliveInstances() { return aliveIns; }

    virtual void setRecordMode(bool isRecordMode) = 0;
    
	bool loadRandomBoard();
	bool loadBoardFromFile(const std::string& fileName);
    
    void run();
};
