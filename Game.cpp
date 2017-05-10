//
//  Game.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Config.h"
#include "Game.h"
#include "GameLoader.h"
#include "Flags.h"
#include "Board.h"
#include "Player.h"
#include "Utils.h"

#define ESC 27

int Game::roundCounter = 0;
int Game::aliveIns = 0;

Game::Game(Player& playerA, Player& playerB, Flags* manager)
    : playerA(playerA), playerB(playerB), gameManager(manager) {
	aliveIns++;
    roundCounter++;

    // Define player keys
	playerA.setKeys("123wxads");
	playerB.setKeys("789imjlk");
}

Game::~Game() {
	aliveIns--;
	if (gameBoard != nullptr)
		delete gameBoard;
}

void Game::setRecordMode(bool isRecordMode) {
    this->isRecordMode = isRecordMode;
}

bool Game::loadRandomBoard() {
	return loadBoard("");
}

bool Game::loadBoardFromFile(const std::string& fileName) {
	return loadBoard(fileName);
}

bool Game::loadBoard(const std::string& fileName) {
	GameLoader loader(playerA, playerB);
	bool fromFile = (fileName.size() > 0);
	bool success = false;
	if (fromFile) {
		gameName = fileName;
		success = loader.loadGameFromFile(fileName);
	}
	else {
		success = loader.loadRandomGame();
	}

	if (success) {
		gameBoard = loader.gameBoard();

		if (!fromFile && isRecordMode) {
			gameName = loader.newRandomFileName();
			loader.saveBoardToFile(gameName);
		}

		drawBoard();
	}
	else {
		loader.printErrors();
	}

	return success;
}

void Game::run() {
	// Verify board was loaded
	bool boardReady = (gameBoard != nullptr);

    while (boardReady) {
		++timeStamp;
		Player& activePlayer = ((timeStamp % 2) ? playerA : playerB);
        handlePlayerTurn(activePlayer);
        if (isGameOver()) break;
		Sleep(100);
		handleKeyboardInput();
    }

	endGame();
}

//*********** Private Helpers ***********//

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
	p.handleLoadedMoveIfNeeded(timeStamp);

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

	// Print message at bottom of board
	const std::string message = std::string("Ship") + std::to_string((int)winner->type()) + 
		" won Ship" + std::to_string((int)loser->type()) + 
		" @ cell (" + cell->getColumn() + "," + std::to_string(cell->getRow()) + ")";
	gameBoard->printMessage(message, false, false);

	loser->setDead();
	winner->moveToCell(cell);
}

void Game::handleKeyboardInput() {
	char ch = 0;
	if (_kbhit()) {
		ch = _getch();
		playerA.notifyKeyHit(ch, timeStamp);
		playerB.notifyKeyHit(ch, timeStamp);
		notifyKeyHit(ch);
	}
}

bool Game::isGameOver() {
    return (gameState != GameState::IN_PROGRESS ||
			playerA.didPlayerWin() ||
            playerB.didPlayerWin() ||
            playerA.didPlayerLose() ||
            playerB.didPlayerLose() ||
			(playerA.didFinishAutoMoves(timeStamp) && playerB.didFinishAutoMoves(timeStamp))
		);
}

void Game::endGame() {
	// Add points to winner
	awardWinner();

	// Print message to board
	std::string message;
	if (playerA.didPlayerWin() || playerB.didPlayerLose()) {
		message = playerA.name() + " won !!!!!";
	}
	else if (playerA.didPlayerLose() || playerB.didPlayerWin()) {
		message = playerB.name() + " won !!!!!";
	}
	else {
		message = "No winners!";
	}
	gameBoard->printMessage(message, true, true);

	// Save move files if record mode
	if (isRecordMode && gameName.size() > 0) {
		playerA.endMoveList(timeStamp);
		playerB.endMoveList(timeStamp);
		GameLoader loader(playerA, playerB);
		loader.savePlayerMovesToFile(gameName);
	}

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
	bool handled;

	do {
		handled = true;
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
			restartGame();
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

void Game::restartGame() {
	playerA.restartGame();
	playerB.restartGame();
	timeStamp = 0;
	drawBoard();
}