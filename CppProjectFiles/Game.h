//
//  Game.h
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include "Board.h"
#include "Player.h"

class Game {
    Player& playerB;
    Player& playerA;
    Board* gameBoard;

public:
    Game(Player& playerA, Player& playerB);

    void run();
    void move();
    void resolveCombat();
};

#endif /* Game_h */
