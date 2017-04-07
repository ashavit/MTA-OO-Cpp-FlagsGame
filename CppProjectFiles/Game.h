#pragma once

#include <stdio.h>
#include "Board.h"
#include "Player.h"

class Game {
    Player& playerB;
    Player& playerA;
    Board* gameBoard;

    void doPlayerTurn(Player& p);
    bool isGameOver();
    
public:
    Game(Player& playerA, Player& playerB);

    void run();
    void move();
    void resolveCombat();
};
