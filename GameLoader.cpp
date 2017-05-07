//
//  Game.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 05/05/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Utils.h"
#include "GameLoader.h"
#include "Player.h"
#include "Board.h"
#include "Cell.h"
#include "PlayerMoves.h"

using namespace std;

#define BOARD_FILE_EXTENSION ".gboard"
#define PLAYER_A_FILE_EXTENSION ".moves-a_small"
#define PLAYER_B_FILE_EXTENSION ".moves-b_small"

#define SET_BIT(NUM, I)             NUM |= ( 1 << (I) )
#define IS_BIT_I_SET(NUM, I)        ( (NUM >> I) & 1 )
#define MASK_WITH_N_LSBS_SET(N)     ( (1 << N) - 1 )

int GameLoader::aliveIns = 0;

GameLoader::GameLoader(Player & playerA, Player & playerB)
	: playerA(playerA), playerB(playerB) {
	aliveIns++;
}

GameLoader::~GameLoader() {
	aliveIns--;
}

bool GameLoader::loadRandomGame()
{
	_gameBoard = loadRandomBoard();
	return true;
}

bool GameLoader::loadGameFromFile(const string& fileName) {
	ifstream* boardFile = openFileToRead(fileName + BOARD_FILE_EXTENSION);
	if (boardFile) {
		_gameBoard = loadBoardFromFile(*boardFile, fileName);

		/// TODO check config flag as well
		// Try to load player moves
		ifstream* playerAFile = openFileToRead(fileName + PLAYER_A_FILE_EXTENSION);
		if (playerAFile) {
			PlayerMoves* movesA = loadPlayerMovesFromFile(*playerAFile);
			if (movesA->moveCount() > 0) {
				playerA.setMoves(movesA);
			}
			closeAndReleaseFile(playerAFile);
		}

		ifstream* playerBFile = openFileToRead(fileName + PLAYER_B_FILE_EXTENSION);
		if (playerBFile) {
			PlayerMoves* movesB = loadPlayerMovesFromFile(*playerBFile);
			if (movesB->moveCount() > 0) {
				playerB.setMoves(movesB);
			}
			closeAndReleaseFile(playerBFile);
		}

		closeAndReleaseFile(boardFile);
	}

	return (_gameBoard != nullptr);
}

void GameLoader::printErrors() {
	cout << endl;
	for (string err : errors)
	{
		cout << err << endl;
	}
	cout << endl << "The game will now exit" << endl;
	waitForAnyKeyToContinue();
}

void GameLoader::savePlayerMovesToFile(const std::string& fileName)
{
	if (!playerA.isAutoMode() && playerA.moves().moveCount()) {
		ofstream* playerAFile = openFileToWrite(fileName + PLAYER_A_FILE_EXTENSION);
		if (playerAFile) {
			saveMovesToFile(*playerAFile, playerA.moves(), 1);
			closeAndReleaseFile(playerAFile);
		}
	}

	if (!playerB.isAutoMode() && playerB.moves().moveCount()) {
		ofstream* playerBFile = openFileToWrite(fileName + PLAYER_B_FILE_EXTENSION);
		if (playerBFile) {
			saveMovesToFile(*playerBFile, playerB.moves(), 0);
			closeAndReleaseFile(playerBFile);
		}
	}
}

Board* GameLoader::loadRandomBoard(UINT width, UINT height) {
	Board *b = new Board(width, height);

	int seas = Board::BoardDensity::REGULAR;
	int forests = Board::BoardDensity::REGULAR;

	b->randomPlaceSpecialCells(CellType::SEA, seas);
	b->randomPlaceSpecialCells(CellType::FORREST, forests);
	b->printBoard();

	b->getRandomCellInRows(1, 5)->setCellType(FLAG_A);
	b->getRandomCellInRows(9, 13)->setCellType(FLAG_B);

	// Init ships
	for (int type = ShipType::SHIP1; type <= ShipType::SHIP3; ++type)
	{
		Cell *c = b->getRandomCellInRows(1, 5);
		Ship *ship = new Ship(playerA, (ShipType)type, c);
		c->setStandingShip(ship);
	}

	for (int type = ShipType::SHIP7; type <= ShipType::SHIP9; ++type)
	{
		Cell *c = b->getRandomCellInRows(9, 13);
		Ship *ship = new Ship(playerB, (ShipType)type, c);
		c->setStandingShip(ship);
	}

	b->printBoard();

	return b;
}

Board* GameLoader::loadBoardFromFile(ifstream& boardFile, const string& fileName, UINT width, UINT height) {
	Board *b = new Board(width, height);

	char validateToolsA = 0, validateToolsB = 0;
	bool isPlayerToolsValidA = true, isPlayerToolsValidB = true;

	for (UINT row = 0; row < height && !boardFile.eof(); ++row) {
		string buff;
		getline(boardFile, buff);
		for (UINT col = 0; col < width; ++col) {
			char ch = buff[col];
			switch (ch) {
			case 'S':
				b->board[col][row]->setCellType(CellType::SEA);
				break;
			case 'T':
				b->board[col][row]->setCellType(CellType::FORREST);
				break;
			case 'A':
				if (IS_BIT_I_SET(validateToolsA, 0)) { isPlayerToolsValidA = false; break; } // Make sure FlagA is set only once
				b->board[col][row]->setCellType(CellType::FLAG_A);
				SET_BIT(validateToolsA, 0);
				break;
			case 'B':
				if (IS_BIT_I_SET(validateToolsB, 0)) { isPlayerToolsValidB = false; break; } // Make sure FlagB is set only once
				b->board[col][row]->setCellType(CellType::FLAG_B);
				SET_BIT(validateToolsB, 0);
				break;
			case '1':
			case '2':
			case '3':
			{
				int bit = ch - '1' + 1;
				if (IS_BIT_I_SET(validateToolsA, bit)) { isPlayerToolsValidA = false; break; } // Make sure each ship is set only once
				Ship *ship = new Ship(playerA, (ShipType)(ch - '0'), b->board[col][row]);
				b->board[col][row]->setStandingShip(ship);
				SET_BIT(validateToolsA, bit);
				break;
			}

			case '7':
			case '8':
			case '9':
			{
				int bit = ch - '7' + 1;
				if (IS_BIT_I_SET(validateToolsB, bit)) { isPlayerToolsValidB = false; break; } // Make sure each ship is set only once
				Ship *ship = new Ship(playerB, (ShipType)(ch - '0'), b->board[col][row]);
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
		delete b;
		return nullptr;
	}

	b->printBoard();
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

void GameLoader::saveMovesToFile(ofstream& movesFile, PlayerMoves& moves, int roundToMod2) {
	map<unsigned long, PlayerMoves::Move*> list = moves.getMovesList();
	for (auto& turn : list) {
		unsigned long ts = turn.first;
		if ((ts % 2) != roundToMod2) { ++ts; } // Make sure time stamp fits player's turn
		movesFile << ts<< ", " << *turn.second << endl;
	}
	movesFile << endl;
}

ifstream* GameLoader::openFileToRead(const std::string fileName) {
	ifstream* file = new ifstream();
	file->open(fileName); // default is text!
	if (!file->good())
	{
		/// TODO: Handle Error
		file->close();
		return nullptr;
	}
	return file;
}

ofstream* GameLoader::openFileToWrite(const std::string fileName) {
	ofstream* file = new ofstream();
	file->open(fileName); // default is text!
	if (!file->good())
	{
		/// TODO: Handle Error
		file->close();
		return nullptr;
	}
	return file;
}

void GameLoader::closeAndReleaseFile(ifstream* file) {
	file->close();
	delete file;
}

void GameLoader::closeAndReleaseFile(ofstream* file) {
	file->close();
	delete file;
}

void GameLoader::addUniqueError(const string& error) {
	for (string str : errors)
	{
		if (error.compare(str) == 0) {
			return;
		}
	}
	errors.push_back(error);
}

