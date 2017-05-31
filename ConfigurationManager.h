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
	bool _saveMode = false;
	int _delay = 20;

public:

	static ConfigurationManager& sharedInstance();

	ConfigurationManager(ConfigurationManager const&) = delete;
	void operator=(ConfigurationManager const&) = delete;

	void setup(int argc, const char* argv[]);

	BoardMode boardMode() const;
	MovesMode movesMode() const;
	std::string path() const;
	bool quietMode() const;
	bool saveMode() const;
	int delay() const;
	int delayBetweenGames() const;
};
