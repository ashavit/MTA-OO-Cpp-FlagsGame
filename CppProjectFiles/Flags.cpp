//
//  Flags.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Flags.h"
#include <iostream>
#include "Utils.h"

using namespace std;

#pragma mark - Ctor

// Init players with default names
Flags::Flags() : playerA(Player("Player A")), playerB(Player("Player B")) { }

Flags::~Flags() {
    // Make sure we do not leave used memory
    if (currentGame != nullptr) {
        delete currentGame;
        currentGame = nullptr;
    }
}

#pragma mark - Public

void Flags::configure(int argc, const char * argv[]) {
    configurationManager.setup(argc, argv);
}

void Flags::run() {
    
    int selection;
	hideCursor();

    do {
        //test_setupStates();

        cout << "Please select an option:" << endl;
        cout << "1. Select Players Name" << endl;
        cout << "2. Begin a new game" << endl;
        cout << "3. Begin a new reverse game" << endl;
        cout << "4. Reset players score" << endl;
        cout << "5. " << (isRecordMode ? "Stop" : "Start") << " record mode" << endl;
        cout << "9. Quit" << endl;
        cin >> selection;
        
        switch (selection) {
            case 1:
                selectPlayerNames();
                break;
            case 2:
                beginNewGame();
                break;
            case 3:
                beginReverseGame();
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
    } while (!shouldExitProgram);
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

#pragma mark - Private

void Flags::selectPlayerNames() {
    playerA.updateName();
    playerB.updateName();
}

void Flags::beginNewGame() {
    currentGame = new Game(playerA, playerB, this);
    startGame();
}

void Flags::beginReverseGame() {
    currentGame = new Game(playerB, playerA, this);
    startGame();
}

void Flags::resetPlayerScores() {
    playerA.resetScore();
    playerB.resetScore();
}

void Flags::toggleRecordMode() {
    isRecordMode = !isRecordMode;
}

void Flags::startGame() {
    currentGame->setRecordMode(isRecordMode);
    currentGame->loadRandomBoard();
    currentGame->run();
    currentGame->run();
}

#pragma mark - Test functions

void Flags::test_setupStates() {

    static int runTwice = 0;
    if (runTwice < 2) {
        playerA.incrementScore(40);
        playerB.incrementScore(70);
        runTwice++;
    }
 
    cout << endl << "player A = " << playerA.name() << ", player b = " << playerB.name() << endl;
    cout << "player A score= " << playerA.score() << ", player B score = " << playerB.score() << endl << endl;

}
