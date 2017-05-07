#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>

class ConfigurationManager {

public:
    
    enum BoardMode {RANDOM, BOARD_FILE};
    enum MovesMode {KEYBOARD, MOVES_FILE};
    
private:
	ConfigurationManager() {};
	~ConfigurationManager() {};

    BoardMode _boardMode = BoardMode::RANDOM;
    MovesMode _movesMode = MovesMode::KEYBOARD;
    std::string _path = "";
    bool _quietMode = false;
    int _delay = 20;
    
public:
    
	static ConfigurationManager& sharedInstance();

	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

    void setup(int argc, const char * argv[]);
    
    BoardMode boardMode();
    MovesMode movesMode();
    std::string path();
    bool quietMode();
    int delay();
    
};
