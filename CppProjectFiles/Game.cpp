//
//  Game.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Game.h"

Game::Game(Player& playerA, Player& playerB)
    : playerA(playerA), playerB(playerB) {

        // Initialize board
        gameBoard = Board::loadRandomBoard();
        
        // Init ships
        for (int type = ShipType::SHIP1; type <= ShipType::SHIP3; ++type )
        {
            Cell *c = gameBoard->getRandomCellInRows(1, 5);
            Ship *ship = new Ship((ShipType)type, c);
            c->setStandingShip(ship);
            playerA.addShip(ship);
        }
        for (int type = ShipType::SHIP7; type <= ShipType::SHIP9; ++type )
        {
            Cell *c = gameBoard->getRandomCellInRows(9, 13);
            Ship *ship = new Ship((ShipType)type, c);
            c->setStandingShip(ship);
            playerB.addShip(ship);
        }
        gameBoard->printBoard();
        
        // TODO: Define player keys
        
}

void Game::run() {
    
    while (1) {
        // TODO: playerA turn

        // TODO: If game is over - break

        // TODO: PlayerB turn

        // TODO: If game is over - break
    }
}

void Game::move() {

}

void Game::resolveCombat() {

}
