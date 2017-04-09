#pragma once

#include <stdio.h>

class Flags;
class Player;
class Board;

class Game {
	enum Awards { WIN = 50, LOSS = 30 };

    Player& playerB;
    Player& playerA;
    Board* gameBoard;
    Flags* gameManager;

	void drawBoard();
    void doPlayerTurn(Player& p);
	void handleKeyboardInput();
    bool isGameOver();
	void endGame();
    void awardWinner();
    
public:
    Game(Player& playerA, Player& playerB, Flags* manager);

    void run();
    void resolveCombat();
};
