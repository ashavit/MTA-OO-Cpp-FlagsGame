#pragma once

#include <stdio.h>

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

    Player& playerB;
    Player& playerA;
    Flags* gameManager;
    Board* gameBoard;
    
    bool isRecordMode = false;
    GameState gameState = GameState::IN_PROGRESS;

	void drawBoard();
    void handlePlayerTurn(Player& p);
	void handleBattle(Ship* shipA, Ship* shipB, Cell* cell);
	void handleKeyboardInput();
    bool isGameOver();
	void endGame();
    void awardWinner();
	void notifyKeyHit(char ch);
	void displaySubMenu();

public:
    Game(Player& playerA, Player& playerB, Flags* manager);
	~Game();
	static int aliveInstances() { return aliveIns; }

    void setRecordMode(bool isRecordMode);
    void loadRandomBoard();
    void run();
    void resolveCombat();
};
