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
            Ship *ship = new Ship(playerA, (ShipType)type, c);
            c->setStandingShip(ship);
        }
        for (int type = ShipType::SHIP7; type <= ShipType::SHIP9; ++type )
        {
            Cell *c = gameBoard->getRandomCellInRows(9, 13);
            Ship *ship = new Ship(playerB, (ShipType)type, c);
            c->setStandingShip(ship);
        }
        gameBoard->printBoard();
        
        // TODO: Define player keys
        
}

void Game::run() {
    
    while (1) {
        // TODO: playerA turn

        // If game is over - break
        if (isGameOver()) break;
        
        // TODO: PlayerB turn

        // If game is over - break
        if (isGameOver()) break;
    }
    
    // TODO: Add points to winner
    
    // TODO: End game
}

void Game::move() {

}

void Game::resolveCombat() {

}

#pragma mark - Private Helpers

bool Game::isGameOver() {
    return (playerA.didPlayerWin() ||
            playerB.didPlayerWin() ||
            playerA.didPlayerLoose() ||
            playerB.didPlayerLoose());
}
