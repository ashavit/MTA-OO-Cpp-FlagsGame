#include "Flags.h"
#include <iostream>
#include "Config.h"
#include "Utils.h"
#include "FileManager.h"
#include "GameLoader.h"
#include "PlayerMoves.h"
#include "ConfigurationManager.h"
#include "BoardDataImpl.h"

using namespace std;

#define POINTS_PER_WIN 10

//*********** Ctor ***********//

// Init players with default names
Flags::Flags() : playerDataA("Player A"), playerDataB("Player B") { }

Flags::~Flags() {
	// Make sure we do not leave used memory
	if (currentGame != nullptr) {
		delete currentGame;
		currentGame = nullptr;
	}
}

//*********** Public ***********//

void Flags::configure(int argc, const char* argv[]) {
	ConfigurationManager& cManager = ConfigurationManager::sharedInstance();
	cManager.setup(argc, argv);

	playerDataA.initPlayer(cManager.movesMode());
	playerDataB.initPlayer(cManager.movesMode());

	if (cManager.boardMode() == ConfigurationManager::BoardMode::BOARD_FILE) {
		FileManager::sharedInstance().loadAvailableFiles(ConfigurationManager::sharedInstance().path());
	}
	if (cManager.saveMode()) {
		isRecordMode = true;
	}
}

void Flags::run() {
	hideCursor();

	if (isAutoModeEnabled()) {
		while (!shouldExitProgram && shouldContinueAutoMode()) {
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

	prepareExitGame();
}

/* Called from Game when a game ends. If _shouldExit=true need to exit program */
void Flags::finishGame(bool _shouldExit) {
	if (currentGame != nullptr) {
		// Add points to winner
		awardWinner();

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
	while (!_kbhit()) {}
	int selection = _getch(); // Clear out last key hit from buffer
	selection -= '0';

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

void Flags::prepareExitGame() {
	playerDataA.freePlayer();
	playerDataB.freePlayer();
	printGameSummary();
}

void Flags::selectPlayerNames() {
	playerDataA.updateName();
	playerDataB.updateName();
}

void Flags::beginKeyboardGame() {
	currentGame = new Game(this, playerDataA.player(), playerDataB.player(), playerDataA.score(), playerDataB.score(), ConfigurationManager::sharedInstance().delay());
	startKeyboardGame();
}

void Flags::beginReverseKeyboardGame() {
	currentGame = new Game(this, playerDataB.player(), playerDataA.player(), playerDataB.score(), playerDataA.score(), ConfigurationManager::sharedInstance().delay());
	startKeyboardGame();
}

void Flags::resetPlayerScores() {
	playerDataA.resetScore();
	playerDataB.resetScore();
}

void Flags::toggleRecordMode() {
	isRecordMode = (!isRecordMode || ConfigurationManager::sharedInstance().saveMode());
	clearScreen();
	cout << "Record mode is now " << (isRecordMode ? "enabled" : "disblaed") << endl << endl;
}

bool Flags::isAutoModeEnabled() {
	ConfigurationManager& cManager = ConfigurationManager::sharedInstance();
	return ((cManager.boardMode() == ConfigurationManager::BOARD_FILE && cManager.movesMode() == ConfigurationManager::MOVES_FILE) ||
		cManager.movesMode() == ConfigurationManager::MOVES_ALGO);
}

bool Flags::shouldContinueAutoMode() const {
	ConfigurationManager& cManager = ConfigurationManager::sharedInstance();
	return ((cManager.limitRounds() && roundCounter < cManager.roundsToPlay()) || // algo limit not reached
		(cManager.boardMode() == ConfigurationManager::BOARD_FILE && // has unplayed board file
			FileManager::sharedInstance().hasMoreBoards()));
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

	if (loadSuccess) {
		++roundCounter;
		currentGame->run();
	} else
		finishGame(true);
}

void Flags::startAutoGame() {
	Game *game = new Game(this, playerDataA.player(), playerDataB.player(), playerDataA.score(), playerDataB.score(), ConfigurationManager::sharedInstance().delay());
	game->setQuietMode(ConfigurationManager::sharedInstance().quietMode());
	currentGame = game;

	bool loadSuccess = FileManager::sharedInstance().hasMoreBoards() &&
		currentGame->loadBoardFromFile(FileManager::sharedInstance().nextFileName());

	if (loadSuccess) {
		++roundCounter;
		currentGame->run();
	} else
		finishGame(true);
}

void Flags::awardWinner() {
	Player* winner = currentGame->gameWinner();
	if (winner == playerDataA.player()) {
		playerDataA.incrementScore(POINTS_PER_WIN);
	}
	else if (winner == playerDataB.player()) {
		playerDataB.incrementScore(POINTS_PER_WIN);
	}
}

void Flags::printGameSummary() const {
	cout << "Game Summary" << endl;
	cout << "A points: " << playerDataA.score() << endl;
	cout << "B points: " << playerDataB.score() << endl;
	waitForAnyKeyToContinue();
}

void Flags::test_printMemoryLeakStatus() {
	cout << endl << "There are " << Game::aliveInstances() << " instances of Game objects alive" << endl;
	cout << "There are " << GameLoader::aliveInstances() << " instances of GameLoader objects alive" << endl;
	cout << "There are " << BoardDataImpl::aliveInstances() << " instances of Board Data objects alive" << endl;
	cout << "There are " << Board::aliveInstances() << " instances of Board objects alive" << endl;
	cout << "There are " << Cell::aliveInstances() << " instances of Cell objects alive" << endl << endl;

	cout << "There are " << Player::aliveInstances() << " instances of Player objects alive" << endl;
	cout << "There are " << Ship::aliveInstances() << " instances of Ship objects alive" << endl;

	cout << "There are " << PlayerMoves::aliveInstances() << " instances of PlayerMoves objects alive" << endl;
	cout << "There are " << PlayerMoves::Move::aliveInstances() << " instances of Move objects alive" << endl;

	waitForAnyKeyToContinue();
}
