//
//  Game.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include <Windows.h>
#include <conio.h>
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

		gameBoard->drawBoard();
        
        // Define player keys
		playerA.setKeys("123wxads");
		playerB.setKeys("789imjlk");
}

void Game::run() {

    while (1) {
        doPlayerTurn(playerA);
        if (isGameOver()) break;
        
		Sleep(400);

        doPlayerTurn(playerB);
        if (isGameOver()) break;

		Sleep(400);

		handleKeyboardInput();
    }

	endGame();
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
				Cell *old = s->cell();
				s->moveToCell(moveTo);
				gameBoard->drawCell(s->cell());
				gameBoard->drawCell(old);
			}
            else if (moveTo->getStandingShip()->owner() != p) { // If Cell is occupied by other player - fight
                /// TODO: Implement Fight
            }
        }
        // Else don't move ship
    }
}

void Game::handleKeyboardInput() {
	char ch = 0;
	if (_kbhit()) {
		ch = _getch();
		playerA.notifyKeyHit(ch);
		playerB.notifyKeyHit(ch);
	}
}

bool Game::isGameOver() {
    return (playerA.didPlayerWin() ||
            playerB.didPlayerWin() ||
            playerA.didPlayerLose() ||
            playerB.didPlayerLose());
}

void Game::endGame() {
	// Delete ships and clear memory
	playerA.clearFleetData();
	playerB.clearFleetData();

	// Add points to winner
	awardWinner();

	// End game
	gameManager->finishGame(false);
}

void Game::awardWinner() {
    if (playerA.didPlayerWin()) {
        playerA.incrementScore(Awards::WIN);
    }
    else if (playerB.didPlayerWin()) {
        playerB.incrementScore(Awards::WIN);
    }
	else if (playerA.didPlayerLose()) {
		playerB.incrementScore(Awards::LOSS);
	}
	else if (playerB.didPlayerLose()) {
		playerA.incrementScore(Awards::LOSS);
	}
}

