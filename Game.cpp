#include <iostream>
#include <cmath>
#include "Config.h"
#include "Game.h"
#include "GameLoader.h"
#include "Flags.h"
#include "Board.h"
#include "FilePlayer.h"
#include "KeyboardPlayer.h"
#include "Utils.h"
#include "ConfigurationManager.h"
#include "BoardDataImpl.h"

#define ESC 27
#define MAX_ALGO_ITTERATIONS 1250

int Game::aliveIns = 0;

Game::Game(Flags* manager, AbstractPlayer* playerA, AbstractPlayer* playerB, int scoreA, int scoreB, int delay)
	: _gameManager(manager), _playerA(playerA), _playerB(playerB), _scoreA(scoreA), _scoreB(scoreB), _delayTurnPeriod(delay) {
	aliveIns++;
	_playerA->setPlayer(1);
	_playerB->setPlayer(2);
}

Game::~Game() {
	aliveIns--;
	if (_boardDataA != nullptr)
		delete _boardDataA;
	if (_boardDataB != nullptr)
		delete _boardDataB;
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

	if (boardReady) {
		_boardDataA = new BoardDataImpl(_gameBoard, 1);
		_boardDataB = new BoardDataImpl(_gameBoard, 2);
		_playerA->init(*_boardDataA);
		_playerB->init(*_boardDataB);

		GameMove move = GameMove(0, 0, 0, 0);
		while (true) {
			++_timeStamp;
			AbstractPlayer* activePlayer = ((_timeStamp % 2) ? _playerA : _playerB);
			move = handlePlayerTurn(activePlayer, move);
			if (isGameOver()) break;
			Sleep(_delayTurnPeriod);
			handleKeyboardInput();
		}
	}

	endGame();
}

AbstractPlayer* Game::gameWinner() const {
	return _winner;
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
	std::cout << _playerA->getName();
	gotoxy(pos, 2);
	std::cout << _scoreA;

	gotoxy(pos, 4);
	std::cout << _playerB->getName();
	gotoxy(pos, 5);
	std::cout << _scoreB;

	std::cout.flush();
}

void Game::drawCell(Cell *cell) const {
	_gameBoard->drawCell(cell);
}

void Game::printBattleResult(std::string result) const {
	_gameBoard->printMessage(result, false, 5, 2);
}

bool Game::isAutoMode() const {
	ConfigurationManager& cManager = ConfigurationManager::sharedInstance();
	return (cManager.movesMode() != ConfigurationManager::MovesMode::KEYBOARD);
}

bool Game::loadBoard(const std::string& fileName) {
	GameLoader loader{};
	bool fromFile = (fileName.size() > 0);
	bool success;
	if (fromFile) {
		_gameName = fileName;
		success = loader.loadGameFromFile(fileName);

		// Check if any of players should load moves from files
		FilePlayer *fPlayerA = dynamic_cast<FilePlayer*>(_playerA);
		FilePlayer *fPlayerB = dynamic_cast<FilePlayer*>(_playerB);
		if (success && (fPlayerA || fPlayerB)) {
			success = loader.loadGameMovesFromFile(fileName, fPlayerA, fPlayerB);
		}
	}
	else {
		success = loader.loadRandomGame();
	}

	if (success) {
		_gameBoard = loader.gameBoard();
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
		notifyKeyHit(ch);

		// Check if players are keyboard
		KeyboardPlayer *kPlayerA = dynamic_cast<KeyboardPlayer*>(_playerA);
		KeyboardPlayer *kPlayerB = dynamic_cast<KeyboardPlayer*>(_playerB);
		if (kPlayerA) {
			kPlayerA->notifyKeyHit(ch, _timeStamp);
		}
		if (kPlayerB) {
			kPlayerB->notifyKeyHit(ch, _timeStamp);
		}
	}
}

GameMove Game::handlePlayerTurn(AbstractPlayer* p, GameMove lastOponentMove) {
	/* Coordinates start from 1,1 */
	FilePlayer* fPlayer = dynamic_cast<FilePlayer*>(p);
	if (fPlayer) {
		fPlayer->setCurrentTimeStamp(_timeStamp);
	}
	GameMove gm = p->play(lastOponentMove);

	// Validate ship moved only by one step
	int deltaX = abs(gm.from_x - gm.to_x);
	int deltaY = abs(gm.from_y - gm.to_y);
	if ((deltaX > 0 && deltaY > 0) || // Try to move in both directions at once
		(deltaX + deltaY > 1)) // try to move more than 1 step a time
		return GameMove(0, 0, 0, 0);

	Cell* moveFrom = _gameBoard->getCellAt(gm.from_x - 1, gm.from_y - 1);
	Ship* s = (moveFrom ? moveFrom->getStandingShip() : nullptr);
	if (s && s->alive()) {
		// Verify ship belongs to player
		if ((p == _playerA && s->type() > ShipType::SHIP3) ||
			(p == _playerB && s->type() < ShipType::SHIP7)) {
			return GameMove(0, 0, 0, 0);
		}
		Cell* moveTo = _gameBoard->getCellAt(gm.to_x - 1, gm.to_y - 1);
		
		if (moveTo != nullptr && s->canMoveToCell(moveTo)) {
			if (moveTo->getStandingShip() == nullptr) { // If cell is empty - move there
				Cell* old = s->cell();
				s->moveToCell(moveTo);
				drawCellIfNeeded(s->cell());
				drawCellIfNeeded(old);
				if ((p == _playerA && moveTo->getCellType() == CellType::FLAG_B) ||
					(p == _playerB && moveTo->getCellType() == CellType::FLAG_A)) {
					// Player found oponent's flag
					_winner = p;
				}
			}
			else if (moveTo->getStandingShip()->type() + s->type() >= 8 &&
				moveTo->getStandingShip()->type() + s->type() <= 12) { // If Cell is occupied by other player - fight
				Cell* old = s->cell();
				handleBattle((p == _playerA ? s : moveTo->getStandingShip()),
					(p == _playerB ? s : moveTo->getStandingShip()),
					moveTo);

				// Redraw both cells no matter who won
				drawCellIfNeeded(moveTo);
				drawCellIfNeeded(old);
			}

			return gm;
		}
	}
	// Else don't move ship
	return GameMove(0, 0, 0, 0);
}

void Game::handleBattle(Ship* shipA, Ship* shipB, Cell* cell) {
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

	// Update counts and check if any player has lost all ships
	if (loser == shipA) {
		--_liveShipsA;
		if (0 == _liveShipsA)
			_winner = _playerB;
	}
	else {
		--_liveShipsB;
		if (0 == _liveShipsB)
			_winner = _playerA;
	}

	loser->setDead();
	winner->moveToCell(cell);
}

bool Game::isGameOver() const {
	bool basicConditions = (_gameState != GameState::IN_PROGRESS || 
		_winner != nullptr);

	FilePlayer *fPlayerA = dynamic_cast<FilePlayer*>(_playerA);
	FilePlayer *fPlayerB = dynamic_cast<FilePlayer*>(_playerB);
	if (fPlayerA && fPlayerB) {
		return (basicConditions ||
			(fPlayerA->didFinishAutoMoves(_timeStamp) &&
				fPlayerB->didFinishAutoMoves(_timeStamp)));
	}
	else if (ConfigurationManager::sharedInstance().movesMode() == ConfigurationManager::MovesMode::MOVES_ALGO &&
		_timeStamp > MAX_ALGO_ITTERATIONS) { // End unfinished algo games passed move limit
		return true;
	} else {
		return basicConditions;
	}
}

void Game::endGame() const {
	// Print message to board
	_gameBoard->printMessage(endGameMessage(), true);
	delayEndGame();

	// End game
	_gameManager->finishGame(_gameState == GameState::ABORT_AND_QUIT);
}

void Game::notifyKeyHit(char ch) {
	if (ch == ESC) {
		displaySubMenu();
	}
}

void Game::displaySubMenu() {
	setTextColor(WHITE);
	clearScreen();

	std::cout << "Game is paused:" << std::endl;
	std::cout << "1. Continue game" << std::endl;
	std::cout << "2. Restart game" << std::endl;
	std::cout << "8. Main menu" << std::endl;
	std::cout << "9. Quit" << std::endl;

	bool handled;
	do {
		handled = true;
		while (!_kbhit()) {}
		int selection = _getch(); // Clear out last key hit from buffer
		selection -= '0';

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
			std::cout << "Undefined action for " << static_cast<char>('0' + selection) << std::endl;
			handled = false;
			break;
		}
	}
	while (!handled);
}

std::string Game::endGameMessage() const {
	if (isAutoMode())
		return endAutoGameMessage();
	else
		return endKeyboardGameMessage();
}

std::string Game::endAutoGameMessage() const {
	std::string message;
	message.append("Game cycle : " + std::to_string(roundNumber()) + "\n");
	message.append("Num moves : " + std::to_string(_timeStamp) + "\n");
	message.append("Winner : ");

	if (gameWinner()) {
		message.append(gameWinner()->getName());
	}
	else {
		message.append("None");
	}

	return message.append("\n");
}

std::string Game::endKeyboardGameMessage() const {
	std::string message;
	if (gameWinner()) {
		message = gameWinner()->getName() + " won !!!!!";
	}
	else {
		message = "No winners!";
	}
	return message;
}

void Game::delayEndGame() const {
	if (isAutoMode()) {
		Sleep(ConfigurationManager::sharedInstance().delayBetweenGames());
	} else {
		waitForAnyKeyToContinue();
	}
}

void Game::unpauseGame() const {
	drawBoardIfNeeded();
}

void Game::restartGame() {
	_timeStamp = 0;
	_gameBoard->restartBoard();
	_playerA->init(*_boardDataA);
	_playerB->init(*_boardDataB);
	drawBoardIfNeeded();
}
