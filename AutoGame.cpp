#include "AutoGame.h"
#include "Player.h"
#include "GameLoader.h"
#include "ConfigurationManager.h"

int AutoGame::aliveIns = 0;

AutoGame::AutoGame(Player& playerA, Player& playerB, Flags* manager, int delay)
	: Game(playerA, playerB, manager, delay) {
	aliveIns++;

	// Define player as auto
	playerA.setAutoMode(true);
	playerB.setAutoMode(true);
}


AutoGame::~AutoGame() {
	aliveIns--;
}

bool AutoGame::loadBoard(const std::string& fileName) {
	// Make sure file name exist
	if (fileName.size() == 0) return false;

	GameLoader loader(playerA, playerB);
	gameName = fileName;
	bool success = loader.loadGameFromFile(fileName);

	if (success) {
		gameBoard = loader.gameBoard();
		drawBoard();
	}
	else {
		loader.printErrors();
	}

	return success;
}

void AutoGame::handleKeyboardInput() {
	if (_kbhit()) {
		char ch = _getch();
		notifyKeyHit(ch);
	}
}

std::string AutoGame::endGameMessage() const {
	return "Not impl yet";
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
	return message;
}

void AutoGame::delayEndGame() const {
	Sleep(ConfigurationManager::sharedInstance().delayBetweenGames());
}

bool AutoGame::isGameOver() const {
	return (Game::isGameOver() ||
		(playerA.didFinishAutoMoves(timeStamp()) &&
			playerB.didFinishAutoMoves(timeStamp())));
}
