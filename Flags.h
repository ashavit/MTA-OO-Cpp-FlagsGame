#pragma once

#include <stdio.h>
#include "ConfigurationManager.h"
#include "Game.h"
#include "Player.h"

class Flags {

    Player playerA;
    Player playerB;
    Game* currentGame = nullptr;
    bool shouldExitProgram = false;
    bool isRecordMode = false;

	static bool isAutoModeEnabled();
    
    void selectPlayerNames();
    void beginKeyboardGame();
    void beginReverseKeyboardGame();
    void resetPlayerScores();
    void toggleRecordMode();

	void startKeyboardGame();
	void startAutoGame();

public:
	Flags();
	~Flags();
	Flags(Flags const&) = delete;
	void operator=(Flags const&) = delete;

	static void configure(int argc, const char * argv[]);
    void run();
    void finishGame(bool shouldExitProgram);
	void displayMenu() const;
	void HandleMenuInput();
};
