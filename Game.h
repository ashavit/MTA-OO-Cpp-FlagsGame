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

	Player& playerA;
	Player& playerB;
    Flags* gameManager = nullptr;
    Board* gameBoard = nullptr;
	std::string gameName;
    
    bool isRecordMode = false;
    GameState gameState = GameState::IN_PROGRESS;
	unsigned long timeStamp = 0;

	bool loadBoard(const std::string& fileName);
	void drawBoard() const;
    void handlePlayerTurn(Player& p) const;
	void handleBattle(Ship* shipA, Ship* shipB, Cell* cell) const;
	void handleKeyboardInput();

	bool isGameOver() const;
	void endGame() const;
    void awardWinner() const;
	void notifyKeyHit(char ch);
	void restartGame();
	void displaySubMenu();

public:
    Game(Player& playerA, Player& playerB, Flags* manager);
	~Game();
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	static int aliveInstances() { return aliveIns; }

    void setRecordMode(bool isRecordMode);
    
    bool loadRandomBoard();
    bool loadBoardFromFile(const std::string& fileName);
    
    void run();
};
