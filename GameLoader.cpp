#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Commons.h"
#include "ConfigurationManager.h"
#include "FileManager.h"
#include "GameLoader.h"
#include "Player.h"
#include "Board.h"
#include "Cell.h"
#include "PlayerMoves.h"

using namespace std;

#define SET_BIT(NUM, I)             NUM |= ( 1 << (I) )
#define IS_BIT_I_SET(NUM, I)        ( (NUM >> I) & 1 )
#define MASK_WITH_N_LSBS_SET(N)     ( (1 << N) - 1 )

#define PLAYER_HOME_ROW_COUNT 5

int GameLoader::aliveIns = 0;

GameLoader::GameLoader(Player& playerA, Player& playerB)
	: playerA(playerA), playerB(playerB) {
	aliveIns++;
}

GameLoader::~GameLoader() {
	aliveIns--;
}

bool GameLoader::loadRandomGame() {
	_gameBoard = loadRandomBoard();
	return true;
}

bool GameLoader::loadGameFromFile(const string& fileName) {
	const string pathFileName = FileManager::sharedInstance().fileNameWithPath(fileName);
	ifstream* boardFile = openFileToRead(pathFileName + BOARD_FILE_EXTENSION);
	if (boardFile) {
		_gameBoard = loadBoardFromFile(*boardFile, fileName);
		closeAndReleaseFile(boardFile);
	}

	return (_gameBoard != nullptr);
}

bool GameLoader::loadGameMovesFromFile(const string& fileName) {
	bool didLoadMoves = false;
	const string pathFileName = FileManager::sharedInstance().fileNameWithPath(fileName);

	// Try to load player moves
	ifstream* playerAFile = openFileToRead(pathFileName + PLAYER_A_FILE_EXTENSION);
	if (playerAFile) {
		PlayerMoves* movesA = loadPlayerMovesFromFile(*playerAFile);
		if (movesA->moveCount() > 0) {
			playerA.setMoves(movesA);
			didLoadMoves = true;
		}
		closeAndReleaseFile(playerAFile);
	}

	ifstream* playerBFile = openFileToRead(pathFileName + PLAYER_B_FILE_EXTENSION);
	if (playerBFile) {
		PlayerMoves* movesB = loadPlayerMovesFromFile(*playerBFile);
		if (movesB->moveCount() > 0) {
			playerB.setMoves(movesB);
			didLoadMoves = true;
		}
		closeAndReleaseFile(playerBFile);
	}

	// Fail if no moves found
	if (!didLoadMoves)
		errors.push_front("Move files were not found for board " + fileName);
	return didLoadMoves;
}

string GameLoader::newRandomFileName() const {
	bool foundName = false;
	int counter = 1;
	string fileName;

	do {
		fileName = RANDOM_FILE_NAME + to_string(counter);
		string fileNameWExt = fileName + BOARD_FILE_EXTENSION;
		++counter;
		if (!ifstream(fileNameWExt)) { // File does not exist
			foundName = true;
		}
	}
	while (!foundName);

	return fileName;
}

void GameLoader::saveBoardToFile(const string& fileName) const {
	const string pathFileName = FileManager::sharedInstance().fileNameWithPath(fileName);
	ofstream* boardFile = openFileToWrite(pathFileName + BOARD_FILE_EXTENSION);
	if (boardFile) {
		saveBoardToFile(*boardFile);
		closeAndReleaseFile(boardFile);
	}
}

void GameLoader::printErrors() {
	cout << endl;
	if (errors.size() > 0) {
		for (string err : errors) {
			cout << err << endl;
		}
		cout << endl << "The game will now exit" << endl;
	}
}

void GameLoader::savePlayerMovesToFile(const string& fileName) const {
	const string pathFileName = FileManager::sharedInstance().fileNameWithPath(fileName);
	if (!playerA.isAutoMode() && playerA.moves().moveCount()) {
		ofstream* playerAFile = openFileToWrite(pathFileName + PLAYER_A_FILE_EXTENSION);
		if (playerAFile) {
			saveMovesToFile(*playerAFile, playerA.moves(), 1);
			closeAndReleaseFile(playerAFile);
		}
	}

	if (!playerB.isAutoMode() && playerB.moves().moveCount()) {
		ofstream* playerBFile = openFileToWrite(pathFileName + PLAYER_B_FILE_EXTENSION);
		if (playerBFile) {
			saveMovesToFile(*playerBFile, playerB.moves(), 0);
			closeAndReleaseFile(playerBFile);
		}
	}
}

Board* GameLoader::loadRandomBoard(UINT width, UINT height) const {
	Board* b = new Board(width, height);

	int playerAHomeMin = 1;
	int playerAHomeMax = PLAYER_HOME_ROW_COUNT - playerAHomeMin + 1;
	int playerBHomeMax = height;
	int playerBHomeMin = playerBHomeMax - PLAYER_HOME_ROW_COUNT + 1;

	int seas = Board::BoardDensity::REGULAR;
	int forests = Board::BoardDensity::REGULAR;

	b->randomPlaceSpecialCells(CellType::SEA, seas);
	b->randomPlaceSpecialCells(CellType::FORREST, forests);

	b->getRandomCellInRows(playerAHomeMin, playerAHomeMax)->setCellType(FLAG_A);
	b->getRandomCellInRows(playerBHomeMin, playerBHomeMax)->setCellType(FLAG_B);

	// Init ships
	for (int type = ShipType::SHIP1; type <= ShipType::SHIP3; ++type) {
		Cell* c = b->getRandomCellInRows(playerAHomeMin, playerAHomeMax);
		Ship* ship = new Ship(playerA, static_cast<ShipType>(type), c);
		c->setStandingShip(ship);
	}

	for (int type = ShipType::SHIP7; type <= ShipType::SHIP9; ++type) {
		Cell* c = b->getRandomCellInRows(playerBHomeMin, playerBHomeMax);
		Ship* ship = new Ship(playerB, static_cast<ShipType>(type), c);
		c->setStandingShip(ship);
	}

	return b;
}

Board* GameLoader::loadBoardFromFile(ifstream& boardFile, const string& fileName, UINT width, UINT height) {
	Board* b = new Board(width, height);

	char validateToolsA = 0, validateToolsB = 0;
	bool isPlayerToolsValidA = true, isPlayerToolsValidB = true;

	for (UINT row = 0; row < height && !boardFile.eof(); ++row) {
		string buff;
		getline(boardFile, buff);
		for (UINT col = 0; col < width; ++col) {
			char ch = buff[col];
			switch (ch) {
			case BOARD_MARK_SEA:
				b->board[col][row]->setCellType(CellType::SEA);
				break;
			case BOARD_MARK_FOREST:
				b->board[col][row]->setCellType(CellType::FORREST);
				break;
			case BOARD_MARK_FLAG_A:
				if (IS_BIT_I_SET(validateToolsA, 0)) {
					isPlayerToolsValidA = false;
					break;
				} // Make sure FlagA is set only once
				b->board[col][row]->setCellType(CellType::FLAG_A);
				SET_BIT(validateToolsA, 0);
				break;
			case BOARD_MARK_FLAG_B:
				if (IS_BIT_I_SET(validateToolsB, 0)) {
					isPlayerToolsValidB = false;
					break;
				} // Make sure FlagB is set only once
				b->board[col][row]->setCellType(CellType::FLAG_B);
				SET_BIT(validateToolsB, 0);
				break;
			case '1':
			case '2':
			case '3':
				{
					int bit = ch - '1' + 1;
					if (IS_BIT_I_SET(validateToolsA, bit)) {
						isPlayerToolsValidA = false;
						break;
					} // Make sure each ship is set only once
					Ship* ship = new Ship(playerA, static_cast<ShipType>(ch - '0'), b->board[col][row]);
					b->board[col][row]->setStandingShip(ship);
					SET_BIT(validateToolsA, bit);
					break;
				}

			case '7':
			case '8':
			case '9':
				{
					int bit = ch - '7' + 1;
					if (IS_BIT_I_SET(validateToolsB, bit)) {
						isPlayerToolsValidB = false;
						break;
					} // Make sure each ship is set only once
					Ship* ship = new Ship(playerB, static_cast<ShipType>(ch - '0'), b->board[col][row]);
					b->board[col][row]->setStandingShip(ship);
					SET_BIT(validateToolsB, bit);
					break;
				}

			case ' ':
				{
					// Leave cell empty
					break;
				}
			default:
				{
					// Handle unknown char - add error after making sure it wan't printed first
					string err = "Wrong character on board : ";
					err.push_back(ch);
					err.append(" in file " + fileName);
					addUniqueError(err);
					break;
				}
			}
		}
	}

	if (!isPlayerToolsValidB || validateToolsB != MASK_WITH_N_LSBS_SET(4)) {
		errors.push_front("Wrong settings for player B tools in file " + fileName);
	}
	if (!isPlayerToolsValidA || validateToolsA != MASK_WITH_N_LSBS_SET(4)) {
		errors.push_front("Wrong settings for player A tools in file " + fileName);
	}

	// If errors were found
	if (errors.size()) {
		playerA.clearFleetData();
		playerB.clearFleetData();
		delete b;
		return nullptr;
	}

	return b;
}

PlayerMoves* GameLoader::loadPlayerMovesFromFile(ifstream& movesFile) {
	PlayerMoves* res = new PlayerMoves();
	long ts, last = 0;
	int ship;
	char dir;

	while (!movesFile.eof()) {

		movesFile >> skipws >> ts;
		movesFile.ignore(100, ',');

		movesFile >> skipws >> ship;
		movesFile.ignore(100, ',');

		movesFile >> skipws >> dir;

		/// TODO: Fix bug of duplicate last move because of not stopping at eof
		if (last < ts) {
			res->addMove(ts, ship, dir);
			last = ts;
		}
	}
	return res;
}

void GameLoader::saveBoardToFile(ofstream& boardFile) const {
	for (UINT row = 0; row < _gameBoard->height; ++row) {
		for (UINT col = 0; col < _gameBoard->width; ++col) {
			Cell* c = _gameBoard->board[col][row];
			if (c->getStandingShip() != nullptr) {
				boardFile << static_cast<int>(c->getStandingShip()->type());
			}
			else {
				switch (c->getCellType()) {
				case FORREST:
					boardFile << BOARD_MARK_FOREST;
					break;
				case SEA:
					boardFile << BOARD_MARK_SEA;
					break;
				case FLAG_A:
					boardFile << BOARD_MARK_FLAG_A;
					break;
				case FLAG_B:
					boardFile << BOARD_MARK_FLAG_B;
					break;

				default:
					boardFile << " ";
					break;
				}
			}
		}
		boardFile << endl;
	}
}

void GameLoader::saveMovesToFile(ofstream& movesFile, PlayerMoves& moves, int roundToMod2) const {
	map<unsigned long, PlayerMoves::Move*> list = moves.getMovesList();
	for (auto& turn : list) {
		unsigned long ts = turn.first;
		if ((ts % 2) != roundToMod2) { ++ts; } // Make sure time stamp fits player's turn
		movesFile << ts << ", " << *turn.second << endl;
	}
	movesFile << endl;
}

ifstream* GameLoader::openFileToRead(const std::string fileName) const {
	ifstream* file = new ifstream();
	file->open(fileName); // default is text!
	if (!file->good()) {
		/// TODO: Handle Error
		file->close();
		return nullptr;
	}
	return file;
}

ofstream* GameLoader::openFileToWrite(const std::string fileName) const {
	ofstream* file = new ofstream();
	file->open(fileName); // default is text!
	if (!file->good()) {
		/// TODO: Handle Error
		file->close();
		return nullptr;
	}
	return file;
}

void GameLoader::closeAndReleaseFile(ifstream* file) const {
	file->close();
	delete file;
}

void GameLoader::closeAndReleaseFile(ofstream* file) const {
	file->close();
	delete file;
}

void GameLoader::addUniqueError(const string& error) {
	for (string str : errors) {
		if (error.compare(str) == 0) {
			return;
		}
	}
	errors.push_back(error);
}
