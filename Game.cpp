#include "Config.h"
#include "Game.h"
#include "GameLoader.h"
#include "Flags.h"
#include "Board.h"
#include "Player.h"
#include "Utils.h"

#define ESC 27

int Game::staticRoundCounter = 0;
int Game::aliveIns = 0;

Game::Game(Player& playerA, Player& playerB, Flags* manager, int delay)
	: gameManager(manager), playerA(playerA), playerB(playerB), delayTurnPeriod(delay) {
	aliveIns++;
	staticRoundCounter++;
}

Game::~Game() {
	aliveIns--;
	if (gameBoard != nullptr)
		delete gameBoard;
}

bool Game::loadRandomBoard() {
	return loadBoard("");
}

bool Game::loadBoardFromFile(const std::string& fileName) {
	return loadBoard(fileName);
}

void Game::run() {
	// Verify board was loaded
	bool boardReady = (gameBoard != nullptr);

	while (boardReady) {
		++_timeStamp;
		Player& activePlayer = ((_timeStamp % 2) ? playerA : playerB);
		handlePlayerTurn(activePlayer);
		if (isGameOver()) break;
		Sleep(delayTurnPeriod);
		handleKeyboardInput();
	}

	endGame();
}

//*********** Private Helpers ***********//

void Game::drawBoard() const {
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

void Game::handlePlayerTurn(Player& p) const {
	p.handleLoadedMoveIfNeeded(_timeStamp);

	for (int i = 0; i < FLEET_SIZE; ++i) {
		Ship* s = p.getShip(i);
		Cell* moveTo = (s->alive() ? gameBoard->getNextCell(s->cell(), s->direction()) : nullptr);

		if (moveTo != nullptr && s->canMoveToCell(moveTo)) {
			if (moveTo->getStandingShip() == nullptr) { // If cell is empty - move there
				Cell* old = s->cell();
				s->moveToCell(moveTo);
				gameBoard->drawCell(s->cell());
				gameBoard->drawCell(old);
			}
			else if (moveTo->getStandingShip()->owner() != p) { // If Cell is occupied by other player - fight
				Cell* old = s->cell();
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

void Game::handleBattle(Ship* shipA, Ship* shipB, Cell* cell) const {
	// PlayerA ships lose in most cases
	Ship *winner = shipB, *loser = shipA;

	switch (shipA->type()) {
	case ShipType::SHIP1:
		{
			// Ship1 always wins except colD or rows 10-13
			if (cell->getColumn() != 'D' &&
				!(cell->getRow() >= 10 && cell->getRow() <= 13)) {
				winner = shipA;
				loser = shipB;
			}
			break;
		}
	case ShipType::SHIP2:
		{
			// Ship2 only wins ships7/8 at col K and rows 3-4
			if (shipB->type() != ShipType::SHIP9 &&
				(cell->getColumn() == 'K' ||
					(cell->getRow() >= 3 && cell->getRow() <= 4))) {
				winner = shipA;
				loser = shipB;
			}
			break;
		}
	case ShipType::SHIP3:
		{
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
	const std::string message = std::string("Ship") + std::to_string(static_cast<int>(winner->type())) +
		" won Ship" + std::to_string(static_cast<int>(loser->type())) +
		" @ cell (" + cell->getColumn() + "," + std::to_string(cell->getRow()) + ")";
	gameBoard->printMessage(message, false, 10, 5);

	loser->setDead();
	winner->moveToCell(cell);
}

bool Game::isGameOver() const {
	return (gameState != GameState::IN_PROGRESS ||
		playerA.didPlayerWin() ||
		playerB.didPlayerWin() ||
		playerA.didPlayerLose() ||
		playerB.didPlayerLose());
}

void Game::endGame() const {
	// Add points to winner
	awardWinner();

	// Print message to board
	gameBoard->printMessage(endGameMessage(), true);
	delayEndGame();

	postGameActions();
	clearPlayerGameData();

	// End game
	gameManager->finishGame(gameState == GameState::ABORT_AND_QUIT);
}

void Game::awardWinner() const {
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

void Game::clearPlayerGameData() const {
	// Delete ships and clear memory
	playerA.clearFleetData();
	playerB.clearFleetData();
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
			unpauseGame();
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
	}
	while (!handled);
}

void Game::restartGame() {
	playerA.restartGame();
	playerB.restartGame();
	_timeStamp = 0;
}
