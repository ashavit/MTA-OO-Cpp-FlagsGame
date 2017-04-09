#pragma once

#include <stdio.h>

class Flags;
class Player;
class Board;

class Game {
    Player& playerB;
    Player& playerA;
    Board* gameBoard;
    Flags* gameManager;

    void doPlayerTurn(Player& p);
	void handleKeyboardInput();
    bool isGameOver();
    void awardWinner();
    
public:
    Game(Player& playerA, Player& playerB, Flags* manager);

    void run();
    void resolveCombat();
};
