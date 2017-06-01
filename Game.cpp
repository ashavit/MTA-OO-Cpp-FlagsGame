#include <iostream>
#include "Config.h"
#include "Game.h"
#include "GameLoader.h"
#include "Flags.h"
#include "Board.h"
#include "Player.h"
#include "Utils.h"
#include "ConfigurationManager.h"

#define ESC 27

int Game::aliveIns = 0;

Game::Game(Player* playerA, Player* playerB, Flags* manager, int delay)
	: _playerA(playerA), _playerB(playerB), _delayTurnPeriod(delay), _gameManager(manager) {
	aliveIns++;

	// TODO: Define player keys
	playerA->setKeys("123wxads");
	playerB->setKeys("789imjlk");
}

Game::~Game() {
	aliveIns--;
	if (_gameBoard != nullptr)
		delete _gameBoard;
}

bool Game::loadRandomBoard() {
	return loadBoard("");
}

bool Game::loadBoardFromFile(const std::string& fileName) {
	return loadBoard(fileName);
}

void Game::run() {
	// Verify board was loaded
	bool boardReady = (_gameBoard != nullptr);

	while (boardReady) {
		++_timeStamp;
		Player* activePlayer = ((_timeStamp % 2) ? _playerA : _playerB);
		handlePlayerTurn(activePlayer);
		if (isGameOver()) break;
		Sleep(_delayTurnPeriod);
		handleKeyboardInput();
	}

	endGame();
}

//*********** Private Helpers ***********//

int Game::roundNumber() const {
	return _gameManager->roundsPlayed();
}

void Game::drawBoard() const {
	_gameBoard->drawBoard();

	// Draw Players Names
	int pos = _gameBoard->getPlayerStatsLocation();
	setTextColor(WHITE);

	gotoxy(pos, 1);
	std::cout << _playerA->name();
	gotoxy(pos, 2);
	std::cout << _playerA->score();

	gotoxy(pos, 4);
	std::cout << _playerB->name();
	gotoxy(pos, 5);
	std::cout << _playerB->score();

	std::cout.flush();
}

void Game::drawCell(Cell *cell) const {
	_gameBoard->drawCell(cell);
}

void Game::printBattleResult(std::string result) const {
	_gameBoard->printMessage(result, false, 5, 2);
}

bool Game::loadBoard(const std::string& fileName) {
	GameLoader loader{};
	bool fromFile = (fileName.size() > 0);
	bool success;
	if (fromFile) {
		_gameName = fileName;
		success = loader.loadGameFromFile(fileName);
	}
	else {
		success = loader.loadRandomGame();
	}

	if (success) {
		_gameBoard = loader.gameBoard();

		if (!fromFile && _isRecordMode) {
			_gameName = loader.newRandomFileName();
			loader.saveBoardToFile(_gameName);
		}

		drawBoardIfNeeded();
	}
	else {
		loader.printErrors();
		delayEndGame();
	}

	return success;
}

void Game::drawBoardIfNeeded() const {
	if (!_isQuietMode)
		drawBoard();
}

void Game::drawCellIfNeeded(Cell* cell) const {
	if (!_isQuietMode)
		drawCell(cell);
}

void Game::printBattleResultIfNeeded(std::string result) const {
	if (!_isQuietMode)
		printBattleResult(result);
}

void Game::handleKeyboardInput() {
	if (_kbhit()) {
		char ch = _getch();
		/// TODO: Check if players are keyboard
//		playerA()->notifyKeyHit(ch, timeStamp());
//		playerB()->notifyKeyHit(ch, timeStamp());
		notifyKeyHit(ch);
	}
}

/// TODO: Amir: Redo this whole thing
void Game::handlePlayerTurn(Player* p) const {
//	p->handleLoadedMoveIfNeeded(_timeStamp);
//
//	for (int i = 0; i < FLEET_SIZE; ++i) {
//		Ship* s = p->getShip(i);
//		Cell* moveTo = (s->alive() ? _gameBoard->getNextCell(s->cell(), s->direction()) : nullptr);
//
//		if (moveTo != nullptr && s->canMoveToCell(moveTo)) {
//			if (moveTo->getStandingShip() == nullptr) { // If cell is empty - move there
//				Cell* old = s->cell();
//				s->moveToCell(moveTo);
//				drawCellIfNeeded(s->cell());
//				drawCellIfNeeded(old);
//			}
//			else if (moveTo->getStandingShip()->type() + s->type() >= 8 && 
//				moveTo->getStandingShip()->type() + s->type() <= 12) { // If Cell is occupied by other player - fight
//				Cell* old = s->cell();
//				handleBattle((p == _playerA ? s : moveTo->getStandingShip()),
//				             (p == _playerB ? s : moveTo->getStandingShip()),
//				             moveTo);
//
//				// Redraw both cells no matter who won
//				drawCellIfNeeded(moveTo);
//				drawCellIfNeeded(old);
//			}
//		}
//		// Else don't move ship
//	}
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
	printBattleResultIfNeeded(message);

	loser->setDead();
	winner->moveToCell(cell);
}

bool Game::isGameOver() const {
	bool basicConditions = (_gameState != GameState::IN_PROGRESS ||
		_playerA->didPlayerWin() ||
		_playerB->didPlayerWin() ||
		_playerA->didPlayerLose() ||
		_playerB->didPlayerLose());
	if (_isQuietMode) {
		return (basicConditions ||
			(_playerA->didFinishAutoMoves(_timeStamp) &&
				_playerB->didFinishAutoMoves(_timeStamp)));
	} else {
		return basicConditions;
	}
}

void Game::endGame() const {
	// Add points to winner
	awardWinner();

	// Print message to board
	_gameBoard->printMessage(endGameMessage(), true);
	delayEndGame();

	postGameActions();
	clearPlayerGameData();

	// End game
	_gameManager->finishGame(_gameState == GameState::ABORT_AND_QUIT);
}

void Game::awardWinner() const {
	if (_playerA->didPlayerWin()) {
		_playerA->incrementScore(Awards::WIN);
	}
	else if (_playerB->didPlayerWin()) {
		_playerB->incrementScore(Awards::WIN);
	}
	else if (_playerA->didPlayerLose()) {
		_playerB->incrementScore(Awards::LOSS);
	}
	else if (_playerB->didPlayerLose()) {
		_playerA->incrementScore(Awards::LOSS);
	}
}

void Game::clearPlayerGameData() const {
	// Delete ships and clear memory
	_playerA->clearFleetData();
	_playerB->clearFleetData();
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
			_gameState = GameState::ABORTED;
			break;
		case 9:
			_gameState = GameState::ABORT_AND_QUIT;
			break;

		default:
			std::cout << "Selection undefined" << std::endl;
			handled = false;
			break;
		}
	}
	while (!handled);
}

std::string Game::endGameMessage() const {
	if (_isQuietMode)
		return endQuietGameMessage();
	else
		return endKeyboardGameMessage();
}

std::string Game::endQuietGameMessage() const {
	std::string message;
	message.append("Game cycle : " + std::to_string(roundNumber()) + "\n");
	message.append("Num moves : " + std::to_string(_timeStamp) + "\n");
	message.append("Winner : ");

	if (_playerA->didPlayerWin() || _playerB->didPlayerLose()) {
		message.append(_playerA->name());
	}
	else if (_playerA->didPlayerLose() || _playerB->didPlayerWin()) {
		message.append(_playerB->name());
	}
	else {
		message.append("None");
	}

	return message.append("\n");
}

std::string Game::endKeyboardGameMessage() const {
	std::string message;
	if (_playerA->didPlayerWin() || _playerB->didPlayerLose()) {
		message = _playerA->name() + " won !!!!!";
	}
	else if (_playerA->didPlayerLose() || _playerB->didPlayerWin()) {
		message = _playerB->name() + " won !!!!!";
	}
	else {
		message = "No winners!";
	}
	return message;
}

void Game::delayEndGame() const {
	if (_isQuietMode) {
		Sleep(ConfigurationManager::sharedInstance().delayBetweenGames());
	} else {
		waitForAnyKeyToContinue();
	}
}

void Game::postGameActions() const {
	// Save move files if record mode
	if (_isRecordMode && _gameName.size() > 0) {
		_playerA->endMoveList(_timeStamp + 1);
		_playerB->endMoveList(_timeStamp + 1);
		GameLoader loader{};
		loader.savePlayerMovesToFile(_gameName);
	}
}

void Game::unpauseGame() const {
	drawBoardIfNeeded();
}

void Game::restartGame() {
	_playerA->restartGame();
	_playerB->restartGame();
	_timeStamp = 0;
	drawBoardIfNeeded();
}
