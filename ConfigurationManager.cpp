//
//  ConfigurationManager.cpp
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 25/04/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#include "ConfigurationManager.h"

#define PARAM_KEY_BOARD "-board"
#define PARAM_KEY_MOVES "-moves"
#define PARAM_VAL_FILE "f"
#define PARAM_KEY_PATH "-path"
#define PARAM_KEY_QUIET "-quiet"
#define PARAM_KEY_DELAY "-delay"

#pragma mark - Ctor

ConfigurationManager& ConfigurationManager::sharedInstance() {
	static ConfigurationManager instance{};
	return instance;
}

void ConfigurationManager::setup(int argc, const char * argv[]) {
    for (int i = 0; i < argc; ++i) {
        std::string key = argv[i];
        if (key == PARAM_KEY_BOARD) {
            _boardMode = ( (strcmp(argv[i+1], PARAM_VAL_FILE) == 0) ? BoardMode::BOARD_FILE : BoardMode::RANDOM);
        }
        else if (key == PARAM_KEY_MOVES) {
            _movesMode = ( (strcmp(argv[i+1], PARAM_VAL_FILE) == 0) ? MovesMode::MOVES_FILE : MovesMode::KEYBOARD);
        }
        else if (key == PARAM_KEY_PATH) {
            _path = argv[i+1];
        }
        else if (key == PARAM_KEY_QUIET) {
            _quietMode = true;
        }
        else if (key == PARAM_KEY_DELAY) {
            _delay = atoi(argv[i+1]);
        }
    }
}

ConfigurationManager::BoardMode ConfigurationManager::boardMode() {
    return _boardMode;
}

ConfigurationManager::MovesMode ConfigurationManager::movesMode() {
    return _movesMode;
}

std::string ConfigurationManager::path() {
    return _path;
}

// This option is relevant only with -board f -moves f otherwise ignored

bool ConfigurationManager::quietMode() {
	// Quiet can be enabled only for all files game
    return _quietMode && (_boardMode == BoardMode::BOARD_FILE) && (_movesMode == MovesMode::MOVES_FILE);
}

int ConfigurationManager::delay() {
    return _delay;
}

