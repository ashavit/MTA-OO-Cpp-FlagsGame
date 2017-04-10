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
#include "Utils.h"

#define ESC 27

Game::Game(Player& playerA, Player& playerB, Flags* manager)
    : playerA(playerA), playerB(playerB), gameManager(manager) {
	aliveIns++;

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

	drawBoard();
        
    // Define player keys
	playerA.setKeys("123wxads");
	playerB.setKeys("789imjlk");
}

Game::~Game() {
	aliveIns--;

	delete gameBoard;
}

int Game::aliveIns = 0;

void Game::run() {

    while (1) {
        handlePlayerTurn(playerA);
        if (isGameOver()) break;
        
		Sleep(400);

        handlePlayerTurn(playerB);
        if (isGameOver()) break;

		Sleep(400);

		handleKeyboardInput();
    }

	endGame();
}

void Game::resolveCombat() {

}

#pragma mark - Private Helpers

void Game::drawBoard() {
	gameBoard->drawBoard();

	// Draw Players Names
	int pos = gameBoard->getPlayerStatsLocation();
	setTextColor(WHITE);

	gotoxy(pos, 1);
	std::cout << playerA.name();
	gotoxy(pos, 2);
	std::cout << playerA.score();

	gotoxy(pos, 4);
	std::cout << playerB.name();
	gotoxy(pos, 5);
	std::cout << playerB.score();

	std::cout.flush();
}

void Game::handlePlayerTurn(Player& p) {
    for (int i = 0; i < FLEET_SIZE; ++i) {
        Ship* s = p.getShip(i);
        Cell *moveTo = (s->alive() ? gameBoard->getNextCell(s->cell(), s->direction()) : nullptr);
        
        if (moveTo != nullptr && s->canMoveToCell(moveTo)) {
            if (moveTo->getStandingShip() == nullptr) { // If cell is empty - move there
				Cell *old = s->cell();
				s->moveToCell(moveTo);
				gameBoard->drawCell(s->cell());
				gameBoard->drawCell(old);
			}
            else if (moveTo->getStandingShip()->owner() != p) { // If Cell is occupied by other player - fight
				Cell *old = s->cell();
				handleBattle((p == playerA ? s : moveTo->getStandingShip()),
					(p == playerB ? s : moveTo->getStandingShip()),
					moveTo);

				// Redraw both cells no matter who won
				gameBoard->drawCell(moveTo);
				gameBoard->drawCell(old);
			}
        }
        // Else don't move ship
    }
}


void Game::handleBattle(Ship* shipA, Ship* shipB, Cell* cell) {
	// PlayerA ships lose in most cases
	Ship *winner = shipB, *loser = shipA;

	switch (shipA->type()) {
		case ShipType::SHIP1: {
			// Ship1 always wins except colD or rows 10-13
			if (cell->getColumn() != 'D' &&
				!(cell->getRow() >= 10 && cell->getRow() <= 13)) {
				winner = shipA;
				loser = shipB;
			}
			break;
		}
		case ShipType::SHIP2: {
			// Ship2 only wins ships7/8 at col K and rows 3-4
			if (shipB->type() != ShipType::SHIP9 &&
				(cell->getColumn() == 'K' ||
				(cell->getRow() >= 3 && cell->getRow() <= 4))) {
				winner = shipA;
				loser = shipB;
			}
			break;
		}
		case ShipType::SHIP3: {
			// Ship3 always wins at col G and row 8
			if (cell->getColumn() == 'G' || cell->getRow() == 8) {
				winner = shipA;
				loser = shipB;
			}
			break;
		}
		default:
			break;
	}

	/// TODO: Print message at bootom of board
	setTextColor(WHITE); clearScreen();
	std::cout << "Ship" << winner->type() << " won Ship" << loser->type() << " @ cell (" << cell->getColumn() << "," << cell->getRow() << ")" << std::endl;
	Sleep(500); drawBoard();

	loser->setDead();
	winner->moveToCell(cell);
}

void Game::handleKeyboardInput() {
	char ch = 0;
	if (_kbhit()) {
		ch = _getch();
		playerA.notifyKeyHit(ch);
		playerB.notifyKeyHit(ch);
		notifyKeyHit(ch);
	}
}

bool Game::isGameOver() {
    return (gameState != GameState::IN_PROGRESS ||
			playerA.didPlayerWin() ||
            playerB.didPlayerWin() ||
            playerA.didPlayerLose() ||
            playerB.didPlayerLose());
}

void Game::endGame() {
	// Add points to winner
	awardWinner();

	// Delete ships and clear memory
	playerA.clearFleetData();
	playerB.clearFleetData();

	// End game
	gameManager->finishGame(gameState == GameState::ABORT_AND_QUIT);
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

void Game::notifyKeyHit(char ch) {
	if (ch == ESC) {
		displaySubMenu();
	}
}

void Game::displaySubMenu() {
	setTextColor(WHITE);
	clearScreen();

	int selection;
	bool handled = true;

	do {
		std::cout << "Game is paused:" << std::endl;
		std::cout << "1. Continue game" << std::endl;
		std::cout << "2. Restart game" << std::endl;
		std::cout << "8. Main menu" << std::endl;
		std::cout << "9. Quit" << std::endl;
		std::cin >> selection;

		switch (selection) {
		case 1:
			drawBoard(); // And continue playing
			break;
		case 2:
			playerA.restartGame();
			playerB.restartGame();
			drawBoard();
			break;
		case 8:
			gameState = GameState::ABORTED;
			break;
		case 9:
			gameState = GameState::ABORT_AND_QUIT;
			break;

		default:
			std::cout << "Selection undefined" << std::endl;
			handled = false;
			break;
		}
	} while (!handled);

}