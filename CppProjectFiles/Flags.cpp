//
//  Flags.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 28/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "Flags.h"
#include <iostream>

using namespace std;


#pragma mark - Ctor

// Init players with default names
Flags::Flags() : playerA(new Player("Player A")), playerB(new Player("Player B")) { }

#pragma mark - Public

void Flags::run() {
    
    int selection;
    do {
        cout << "Please select an option:" << endl;
        cout << "1. Select Players Name" << endl;
        cout << "2. Begin a new game" << endl;
        cout << "3. Begin a new reverse game" << endl;
        cout << "4. Reset players score" << endl;
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
    shouldExitProgram = _shouldExit;
    currentGame = nullptr;
}

#pragma mark - Private

void Flags::selectPlayerNames() {
    string name;
    cout << "Please enter name for Player A:" << endl;
    cin >> name;
    playerA->updateName(name);

    cout << "Please enter name for Player B:" << endl;
    cin >> name;
    playerB->updateName(name);
}

void Flags::beginNewGame() {
    /// TODO: Amir
}

void Flags::beginReverseGame() {
    /// TODO: Amir
}

void Flags::resetPlayerScores() {
    playerA->resetScore();
    playerB->resetScore();
}


