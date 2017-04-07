#pragma once

#include <stdio.h>
#include "Game.h"
#include "Player.h"

class Flags {

    Player playerA;
    Player playerB;
    Game* currentGame;
    bool shouldExitProgram = false;
    
    void selectPlayerNames();
    void beginNewGame();
    void beginReverseGame();
    void resetPlayerScores();
    
    void test_setupStates();

public:
    
    Flags();
    ~Flags();
    
    void run();
    void finishGame(bool shouldExitProgram);

};
