#pragma once

#include <string>

class Flags;
class Player;
class Ship;
class Board;
class Cell;

class Game
{
	static int aliveIns;

	enum Awards
	{
		WIN = 10,
		LOSS = 10
	};

	enum GameState
	{
		IN_PROGRESS,
		ABORTED,
		ABORT_AND_QUIT
	};

	Flags* _gameManager = nullptr;
	Player* _playerA;
	Player* _playerB;
	int _scoreA;
	int _scoreB;
	int _delayTurnPeriod;
	Board* _gameBoard = nullptr;
	std::string _gameName;

	GameState _gameState = GameState::IN_PROGRESS;
	unsigned long _timeStamp = 0;
	bool _isRecordMode = false;
	bool _isQuietMode = false;

	bool loadBoard(const std::string& fileName);
	void drawBoardIfNeeded() const;
	void drawBoard() const;
	void drawCell(Cell* cell) const;
	void drawCellIfNeeded(Cell* cell) const;
	void printBattleResult(std::string result) const;
	void printBattleResultIfNeeded(std::string result) const;

	void handleKeyboardInput();
	void notifyKeyHit(char ch);
	void handlePlayerTurn(Player* p) const;
	void handleBattle(Ship* shipA, Ship* shipB, Cell* cell) const;
	void endGame() const;
	void displaySubMenu();

	bool isGameOver() const;
	int roundNumber() const;
	std::string endGameMessage() const;
	std::string Game::endQuietGameMessage() const;
	std::string Game::endKeyboardGameMessage() const;
	void delayEndGame() const;
	void postGameActions() const;
	void unpauseGame() const;
	void restartGame();

public:
	Game(Flags* manager, Player* playerA, Player* playerB, int scoreA, int scoreB, int delay);
	virtual ~Game();
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	static int aliveInstances() { return aliveIns; }

	void setRecordMode(bool isRecordMode) { _isRecordMode = isRecordMode; };
	void setQuietMode(bool isQuiet) {
		_isQuietMode = isQuiet;
	}

	bool loadRandomBoard();
	bool loadBoardFromFile(const std::string& fileName);

	void run();
	Player* gameWinner() const;
};

