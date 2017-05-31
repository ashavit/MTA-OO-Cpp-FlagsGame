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

	Player& _playerA;
	Player& _playerB;
	int _delayTurnPeriod;
	Board* _gameBoard = nullptr;
	std::string _gameName;
	Flags* _gameManager = nullptr;

	GameState _gameState = GameState::IN_PROGRESS;
	unsigned long _timeStamp = 0;

	void handlePlayerTurn(Player& p) const;
	void handleBattle(Ship* shipA, Ship* shipB, Cell* cell) const;
	void endGame() const;
	void awardWinner() const;
	void displaySubMenu();

protected:
	Player& playerA() const { return _playerA; };
	Player& playerB() const { return _playerB; };
	int delayTurnPeriod() const { return _delayTurnPeriod; };
	Board* gameBoard() const { return _gameBoard; }
	void setGameBoard(Board *board) { _gameBoard = board; }
	std::string gameName() const { return _gameName; };
	void setGameName(std::string name) { _gameName = name; }
	int roundNumber() const;

	void drawBoard() const;
	void drawCell(Cell* cell) const;
	virtual void printBattleResult(std::string result) const;
	void notifyKeyHit(char ch);
	void clearPlayerGameData() const;
	unsigned long timeStamp() const { return _timeStamp; };

	virtual bool loadBoard(const std::string& fileName) = 0;
	virtual void drawCellIfNeeded(Cell* cell) const = 0;
	virtual void printBattleResultIfNeeded(std::string result) const = 0;
	virtual void handleKeyboardInput() = 0;
	virtual std::string endGameMessage() const = 0;
	virtual void delayEndGame() const = 0;
	virtual void postGameActions() const = 0;
	virtual void unpauseGame() const = 0;

	virtual bool isGameOver() const;
	virtual void restartGame();

public:
	Game(Player& playerA, Player& playerB, Flags* manager, int delay);
	virtual ~Game();
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	static int aliveInstances() { return aliveIns; }

	virtual void setRecordMode(bool isRecordMode) = 0;

	bool loadRandomBoard();
	bool loadBoardFromFile(const std::string& fileName);

	void run();
};

