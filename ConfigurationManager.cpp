#include "ConfigurationManager.h"

#define PARAM_KEY_BOARD "-board"
#define PARAM_VAL_BOARD_FILE 'f'
#define PARAM_KEY_MOVES "-moves"
#define PARAM_VAL_MOVES_FILE 'f'
#define PARAM_VAL_MOVES_ALGO 'a'
#define PARAM_KEY_PATH "-path"
#define PARAM_KEY_QUIET "-quiet"
#define PARAM_KEY_SAVE "-save"
#define PARAM_KEY_DELAY "-delay"

#define MULTIPLY_DELAY_PERIOD 50

//*********** Ctor ***********//

ConfigurationManager& ConfigurationManager::sharedInstance() {
	static ConfigurationManager instance{};
	return instance;
}

void ConfigurationManager::setup(int argc, const char* argv[]) {
	for (int i = 0; i < argc; ++i) {
		std::string key = argv[i];
		if (key == PARAM_KEY_BOARD) {
			// Make sure not out of bounds
			_boardMode = (((i + 1 < argc) && (argv[i + 1][0] == PARAM_VAL_BOARD_FILE)) ? BoardMode::BOARD_FILE : BoardMode::RANDOM);
		}
		else if (key == PARAM_KEY_MOVES) {
			_movesMode = MovesMode::KEYBOARD;
			// Make sure not out of bounds
			if (i + 1 < argc) {
				char param = argv[i + 1][0];
				if (param == PARAM_VAL_MOVES_FILE)
					_movesMode = MovesMode::MOVES_FILE;
				else if (param == PARAM_VAL_MOVES_ALGO)
					_movesMode = MovesMode::MOVES_ALGO;
			}

		}
		else if (key == PARAM_KEY_PATH) {
			// Make sure not out of bounds
			if (i + 1 < argc)
				_path = argv[i + 1];
		}
		else if (key == PARAM_KEY_QUIET) {
			_quietMode = true;
		}
		else if (key == PARAM_KEY_SAVE) {
			_saveMode = true;
		}
		else if (key == PARAM_KEY_DELAY) {
			// Make sure not out of bounds
			if (i + 1 < argc)
				_delay = atoi(argv[i + 1]);
		}
	}
}

ConfigurationManager::BoardMode ConfigurationManager::boardMode() const {
	return _boardMode;
}

ConfigurationManager::MovesMode ConfigurationManager::movesMode() const {
	// File mode for moves can be enabled only if board is read from file as well
	return (_boardMode == BoardMode::BOARD_FILE ? _movesMode : MovesMode::KEYBOARD);
}

std::string ConfigurationManager::path() const {
	return _path;
}

// This option is relevant only with -board f -moves f otherwise ignored
bool ConfigurationManager::quietMode() const {
	// Quiet can be enabled only if board and moves are loaded from files
	return _quietMode && (_boardMode == BoardMode::BOARD_FILE) && (_movesMode == MovesMode::MOVES_FILE || _movesMode == MovesMode::MOVES_ALGO);
}

// This option is relevant only with -moves a otherwise ignored
bool ConfigurationManager::saveMode() const {
	// Save can be enabled only if moves are performed from alogritm
	return (_saveMode && _movesMode == MovesMode::MOVES_ALGO);
}

int ConfigurationManager::delay() const {
	return quietMode() ? 0 : _delay;
}

int ConfigurationManager::delayBetweenGames() const {
	return _delay * MULTIPLY_DELAY_PERIOD;
}
