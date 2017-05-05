//
//  main.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 25/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include <iostream>

using namespace std;

#include "Flags.h"
#include "ConfigurationManager.h"
#include "Board.h"

void test_printMemoryLeakStatus() {
	cout << endl << "There are " << Game::aliveInstances() << " instances of Game objects alive" << endl;
	cout << "There are " << Board::aliveInstances() << " instances of Board objects alive" << endl;
	cout << "There are " << Cell::aliveInstances() << " instances of Cell objects alive" << endl << endl;

	cout << "There are " << Player::aliveInstances() << " instances of Player objects alive" << endl;
	cout << "There are " << Ship::aliveInstances() << " instances of Ship objects alive" << endl;

    cout << "There are " << ConfigurationManager::aliveInstances() << " instances of ConfigurationManager objects alive" << endl;
    
	char t;
	cin >> t;
}

int main(int argc, const char * argv[]) {

    Flags flags;
    
    if (argc > 1) {
        flags.configure(argc-1, argv + 1);
    }
    flags.run();

	if (DEBUG)
		test_printMemoryLeakStatus();
    return 0;
}
