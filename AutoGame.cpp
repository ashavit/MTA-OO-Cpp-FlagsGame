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

void AutoGame::drawBoardIfNeeded() const {
	if (!isQuietMode)
		drawBoard();
}

bool AutoGame::loadBoard(const std::string& fileName) {
	// Make sure file name exist
	if (fileName.size() == 0) return false;

	GameLoader loader(playerA, playerB);
	gameName = fileName;
	bool success = loader.loadGameFromFile(fileName);

	if (success) {
		gameBoard = loader.gameBoard();
		success = loader.loadGameMovesFromFile(fileName);
		if (success)
			drawBoardIfNeeded();
	}

	if (!success) {
		loader.printErrors();
		clearPlayerGameData();
		delayEndGame();
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
	std::string message;
	message.append("Game cycle : " + std::to_string(roundCounter()) + "\n");
	message.append("Num moves : " + std::to_string(timeStamp()) + "\n");
	message.append("Winner : ");

	if (playerA.didPlayerWin() || playerB.didPlayerLose()) {
		message.append(playerA.name());
	}
	else if (playerA.didPlayerLose() || playerB.didPlayerWin()) {
		message.append(playerB.name());
	}
	else {
		message.append("None");
	}

	return message.append("\n");
}

void AutoGame::delayEndGame() const {
	Sleep(ConfigurationManager::sharedInstance().delayBetweenGames());
}

void AutoGame::unpauseGame() const {
	drawBoardIfNeeded();
}

bool AutoGame::isGameOver() const {
	return (Game::isGameOver() ||
		(playerA.didFinishAutoMoves(timeStamp()) &&
			playerB.didFinishAutoMoves(timeStamp())));
}
