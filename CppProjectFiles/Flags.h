#pragma once

#include <stdio.h>
#include "Game.h"
#include "Player.h"

class Flags {

    Player playerA;
    Player playerB;
    Game* currentGame = nullptr;
    bool shouldExitProgram = false;
    bool isRecordMode = false;
    
    void selectPlayerNames();
    void beginNewGame();
    void beginReverseGame();
    void resetPlayerScores();
    void toggleRecordMode();

    void startGame();

    void test_setupStates();

public:
    
    Flags();
    ~Flags();
    
    void run();
    void finishGame(bool shouldExitProgram);

};
