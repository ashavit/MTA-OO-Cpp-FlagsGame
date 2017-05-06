#pragma once

#include <iostream>
#include <map>

class Ship;
enum class Direction;

class PlayerMoves
{
	static int aliveIns;

	class Move {
		static int aliveIns;

		int _shipType;
		char _direction;

	public:
		Move(int type, char dir) : _shipType(type), _direction(dir) { aliveIns++; }
		~Move() { aliveIns--; };

		int shipType() { return _shipType; }
		char direction() { return _direction; }
	};

	std::map<unsigned long, Move*> _moves;

public:
	PlayerMoves();
	~PlayerMoves();

	void addMove(unsigned long ts, int ship, char dir);
	void addMove(unsigned long ts, Ship& ship, Direction dir);

	unsigned int moveCount() { return _moves.size(); }
	bool isEnded(unsigned long ts);

};

