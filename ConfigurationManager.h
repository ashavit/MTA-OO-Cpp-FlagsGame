#pragma once

#include <string>

class ConfigurationManager
{
public:

	enum BoardMode
	{
		RANDOM,
		BOARD_FILE
	};

	enum MovesMode
	{
		KEYBOARD,
		MOVES_FILE,
		MOVES_ALGO
	};

private:
	ConfigurationManager() {};
	~ConfigurationManager() {};

	BoardMode _boardMode = BoardMode::RANDOM;
	MovesMode _movesMode = MovesMode::KEYBOARD;
	std::string _path = "";
	bool _quietMode = false;
	int _delay = 20;
	unsigned int _rounds = 10;

public:

	static ConfigurationManager& sharedInstance();

	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

	void setup(int argc, const char* argv[]);

	BoardMode boardMode() const;
	MovesMode movesMode() const;
	std::string path() const;
	bool quietMode() const;
	int delay() const;
	int delayBetweenGames() const;

	bool limitRounds() const;
	unsigned int roundsToPlay() const;
};
