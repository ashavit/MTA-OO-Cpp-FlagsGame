#include "KeyboardGame.h"
#include "Player.h"
#include "GameLoader.h"
#include "Utils.h"

int KeyboardGame::aliveIns = 0;

KeyboardGame::KeyboardGame(Player& playerA, Player& playerB, Flags* manager, int delay)
	: Game(playerA, playerB, manager, delay) {
	aliveIns++;

	// Define player keys
	playerA.setKeys("123wxads");
	playerB.setKeys("789imjlk");
}

KeyboardGame::~KeyboardGame() {
	aliveIns--;
}

void KeyboardGame::setRecordMode(bool isRecordMode) {
	isRecordMode = isRecordMode;
}

bool KeyboardGame::loadBoard(const std::string& fileName) {
	GameLoader loader(playerA(), playerB());
	bool fromFile = (fileName.size() > 0);
	bool success;
	if (fromFile) {
		setGameName(fileName);
		success = loader.loadGameFromFile(fileName);
	}
	else {
		success = loader.loadRandomGame();
	}

	if (success) {
		setGameBoard(loader.gameBoard());

		if (!fromFile && isRecordMode) {
			setGameName(loader.newRandomFileName());
			loader.saveBoardToFile(gameName());
		}

		drawBoard();
	}
	else {
		loader.printErrors();
		delayEndGame();
	}

	return success;
}

void KeyboardGame::handleKeyboardInput() {
	if (_kbhit()) {
		char ch = _getch();
		playerA().notifyKeyHit(ch, timeStamp());
		playerB().notifyKeyHit(ch, timeStamp());
		notifyKeyHit(ch);
	}
}

std::string KeyboardGame::endGameMessage() const {
	std::string message;
	if (playerA().didPlayerWin() || playerB().didPlayerLose()) {
		message = playerA().name() + " won !!!!!";
	}
	else if (playerA().didPlayerLose() || playerB().didPlayerWin()) {
		message = playerB().name() + " won !!!!!";
	}
	else {
		message = "No winners!";
	}
	return message;
}

void KeyboardGame::delayEndGame() const {
	waitForAnyKeyToContinue();
}

void KeyboardGame::postGameActions() const {
	// Save move files if record mode
	if (isRecordMode && gameName().size() > 0) {
		playerA().endMoveList(timeStamp());
		playerB().endMoveList(timeStamp());
		GameLoader loader(playerA(), playerB());
		loader.savePlayerMovesToFile(gameName());
	}
}

void KeyboardGame::restartGame() {
	Game::restartGame();
	drawBoard();
}
