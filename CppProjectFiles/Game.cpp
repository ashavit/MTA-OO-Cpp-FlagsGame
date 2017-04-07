//
//  Game.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Game.h"
#include "Flags.h"
#include "Board.h"
#include "Player.h"

Game::Game(Player& playerA, Player& playerB, Flags* manager)
    : playerA(playerA), playerB(playerB), gameManager(manager) {

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
        doPlayerTurn(playerA);
        if (isGameOver()) break;
        
        /// TODO: Wait
        
        doPlayerTurn(playerB);
        if (isGameOver()) break;

        /// TODO: Wait
        
        /// TODO: Get KB Input
    }
    
    // Add points to winner
    awardWinner();
    
    // TODO: End game
    gameManager->finishGame(false);
}

void Game::resolveCombat() {

}

#pragma mark - Private Helpers

void Game::doPlayerTurn(Player& p) {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        Ship* s = p.getShip(i);
        Cell *moveTo = gameBoard->getNextCell(s->cell(), s->direction());
        
        if (moveTo != nullptr && s->canMoveToCell(moveTo)) {
            if (moveTo->getStandingShip() == nullptr) { // If cell is empty - move there
                s->moveToCell(moveTo);
            }
            else if (moveTo->getStandingShip()->owner() != p) { // If Cell is occupied by other player - fight
                /// TODO: Implement Fight
            }
        }
        // Else don't move ship
    }
}

bool Game::isGameOver() {
    return (playerA.didPlayerWin() ||
            playerB.didPlayerWin() ||
            playerA.didPlayerLoose() ||
            playerB.didPlayerLoose());
}

void Game::awardWinner() {
    /// TODO: enum scores or const
    int points = 50;
    if (playerA.didPlayerWin()) {
        playerA.incrementScore(points);
    }
    else if (playerB.didPlayerWin()) {
        playerB.incrementScore(points);
    }
}

