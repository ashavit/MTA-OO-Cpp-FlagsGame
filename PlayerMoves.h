#pragma once

#include <iostream>
#include <map>

class Ship;
enum class Direction;

class PlayerMoves
{
public:
	class Move {
		static int aliveIns;

		int _shipType;
		char _direction;

		friend std::ostream& operator<<(std::ostream& out, const Move& move);

	public:
		Move(int type, char dir) : _shipType(type), _direction(dir) { aliveIns++; }
		~Move() { aliveIns--; };
		static int aliveInstances() { return aliveIns; }

		int shipType() const { return _shipType; }
		Direction direction() const;
		bool isMoveValid();
	};

private:
	static int aliveIns;

	std::map<unsigned long, Move*> _moves;

public:
	PlayerMoves();
	~PlayerMoves();
	static int aliveInstances() { return aliveIns; }

	void addMove(unsigned long ts, int ship, char dir);
	void addMove(unsigned long ts, Ship& ship, Direction dir);
	const Move* const getMove(unsigned long timeStamp);

	std::map<unsigned long, Move*> getMovesList() {	return _moves; }
	unsigned int moveCount() { return _moves.size(); }
	bool isEnded(unsigned long ts);

	static Direction directionFromChar(char _direction);
	static char charFromDirection(Direction _direction);
};

