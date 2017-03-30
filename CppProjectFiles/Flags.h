//
//  Flags.h
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#ifndef Flags_h
#define Flags_h

#include <stdio.h>
#include "Game.h"
#include "Player.h"

class Flags {

    Player* playerA;
    Player* playerB;
    Game* currentGame;
    bool shouldExitProgram = false;
    
    void selectPlayerNames();
    void beginNewGame();
    void beginReverseGame();
    void resetPlayerScores();

public:
    
    Flags();
    
    void run();
    void finishGame(bool shouldExitProgram);

};

#endif /* Flags_h */
