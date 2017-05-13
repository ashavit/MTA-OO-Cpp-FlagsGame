#include <iostream>
#include "Flags.h"
#include "Utils.h"
#include "GameLoader.h"
#include "AutoGame.h"
#include "KeyboardGame.h"
#include "Board.h"
#include "PlayerMoves.h"

using namespace std;

void test_printMemoryLeakStatus() {
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

int main(int argc, const char* argv[]) {

	Flags flags;

	if (argc > 1) {
		flags.configure(argc - 1, argv + 1);
	}
	flags.run();

	if (DEBUG)
		test_printMemoryLeakStatus();
	return 0;
}
