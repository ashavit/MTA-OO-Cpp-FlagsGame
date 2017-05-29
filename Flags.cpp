#include "Flags.h"
#include <iostream>
#include "Config.h"
#include "Utils.h"
#include "FileManager.h"
#include "AutoGame.h"
#include "KeyboardGame.h"
#include "GameLoader.h"
#include "PlayerMoves.h"
#include "ConfigurationManager.h"

using namespace std;

//*********** Ctor ***********//

// Init players with default names
Flags::Flags() : playerA("Player A"), playerB("Player B") { }

Flags::~Flags() {
	// Make sure we do not leave used memory
	if (currentGame != nullptr) {
		delete currentGame;
		currentGame = nullptr;
	}
}

//*********** Public ***********//

void Flags::configure(int argc, const char* argv[]) {
	ConfigurationManager::sharedInstance().setup(argc, argv);

	if (ConfigurationManager::sharedInstance().boardMode() == ConfigurationManager::BoardMode::BOARD_FILE) {
		FileManager::sharedInstance().loadAvailableFiles(ConfigurationManager::sharedInstance().path());
	}
}

void Flags::run() {

	hideCursor();

	if (isAutoModeEnabled()) {
		while (!shouldExitProgram && FileManager::sharedInstance().hasMoreBoards()) {
			startAutoGame();
		}
	}
	else {
		do {
			displayMenu();
			HandleMenuInput();
		}
		while (!shouldExitProgram);
	}

	printGameSummary();
}

/* Called from Game when a game ends. If _shouldExit=true need to exit program */
void Flags::finishGame(bool _shouldExit) {
	if (currentGame != nullptr) {
		delete currentGame;
		currentGame = nullptr;
	}
	shouldExitProgram = _shouldExit;
	setTextColor(WHITE);
	clearScreen();
}

//*********** Private ***********//

void Flags::displayMenu() const {
	clearScreen();
	cout << "Please select an option:" << endl;
	cout << "1. Select Players Name" << endl;
	cout << "2. Begin a new game" << endl;
	cout << "3. Begin a new reverse game" << endl;
	cout << "4. Reset players score" << endl;
	cout << "5. " << (isRecordMode ? "Stop" : "Start") << " record mode" << endl;
	cout << "9. Quit" << endl;
}

void Flags::HandleMenuInput() {
	int selection;
	cin >> selection;

	switch (selection) {
	case 1:
		selectPlayerNames();
		break;
	case 2:
		beginKeyboardGame();
		break;
	case 3:
		beginReverseKeyboardGame();
		break;
	case 4:
		resetPlayerScores();
		break;
	case 5:
		toggleRecordMode();
		break;
	case 9:
		finishGame(true);
		break;

	default:
		cout << "Selection undefined" << endl;
		break;
	}
}

void Flags::selectPlayerNames() {
	playerA.updateName();
	playerB.updateName();
}

void Flags::beginKeyboardGame() {
	currentGame = new KeyboardGame(playerA, playerB, this, ConfigurationManager::sharedInstance().delay());
	startKeyboardGame();
}

void Flags::beginReverseKeyboardGame() {
	currentGame = new KeyboardGame(playerB, playerA, this, ConfigurationManager::sharedInstance().delay());
	startKeyboardGame();
}

void Flags::resetPlayerScores() {
	playerA.resetScore();
	playerB.resetScore();
}

void Flags::toggleRecordMode() {
	isRecordMode = !isRecordMode;
	clearScreen();
	cout << "Record mode is now " << (isRecordMode ? "enabled" : "disblaed") << endl << endl;
}

bool Flags::isAutoModeEnabled() {
	ConfigurationManager& cManager = ConfigurationManager::sharedInstance();
	return cManager.boardMode() == ConfigurationManager::BOARD_FILE && cManager.movesMode() == ConfigurationManager::MOVES_FILE;
}

void Flags::startKeyboardGame() {
	currentGame->setRecordMode(isRecordMode);

	bool loadSuccess;
	if (ConfigurationManager::sharedInstance().boardMode() == ConfigurationManager::BoardMode::BOARD_FILE) {
		loadSuccess = FileManager::sharedInstance().hasMoreBoards() &&
			currentGame->loadBoardFromFile(FileManager::sharedInstance().nextFileName());
	}
	else {
		loadSuccess = currentGame->loadRandomBoard();
	}

	if (loadSuccess)
		currentGame->run();
	else
		finishGame(true);
}

void Flags::startAutoGame() {
	AutoGame *game = new AutoGame(playerA, playerB, this, ConfigurationManager::sharedInstance().delay());
	game->setQuietMode(ConfigurationManager::sharedInstance().quietMode());
	currentGame = game;

	bool loadSuccess = FileManager::sharedInstance().hasMoreBoards() &&
		currentGame->loadBoardFromFile(FileManager::sharedInstance().nextFileName());

	if (loadSuccess)
		currentGame->run();
	else
		finishGame(true);
}

void Flags::printGameSummary() const {
	cout << "Game Summary" << endl;
	cout << "A points: " << playerA.score() << endl;
	cout << "B points: " << playerB.score() << endl;
	waitForAnyKeyToContinue();
}

void Flags::test_printMemoryLeakStatus() {
	cout << endl << "There are " << Game::aliveInstances() << " instances of Game objects alive" << endl;
	cout << endl << "There are " << KeyboardGame::aliveInstances() << " instances of KeybaordGame objects alive" << endl;
	cout << endl << "There are " << AutoGame::aliveInstances() << " instances of AutoGame objects alive" << endl;
	cout << "There are " << GameLoader::aliveInstances() << " instances of GameLoader objects alive" << endl;
	cout << "There are " << Board::aliveInstances() << " instances of Board objects alive" << endl;
	cout << "There are " << Cell::aliveInstances() << " instances of Cell objects alive" << endl << endl;

	cout << "There are " << Player::aliveInstances() << " instances of Player objects alive" << endl;
	cout << "There are " << Ship::aliveInstances() << " instances of Ship objects alive" << endl;

	cout << "There are " << PlayerMoves::aliveInstances() << " instances of PlayerMoves objects alive" << endl;
	cout << "There are " << PlayerMoves::Move::aliveInstances() << " instances of Move objects alive" << endl;

	waitForAnyKeyToContinue();
}
