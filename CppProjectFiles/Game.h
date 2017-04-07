#pragma once

#include <stdio.h>

class Player;
class Board;

class Game {
    Player& playerB;
    Player& playerA;
    Board* gameBoard;

    void doPlayerTurn(Player& p);
    bool isGameOver();
    void awardWinner();
    
public:
    Game(Player& playerA, Player& playerB);

    void run();
    void resolveCombat();
};
