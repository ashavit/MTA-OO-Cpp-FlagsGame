#pragma once

#include <iostream>
#include <list>

class PlayerMoves
{
	static int aliveIns;

	class Move {
		static int aliveIns;

		long _timeStamp;
		int _shipType;
		char _direction;

	public:
		Move(long ts, int type, char dir) : _timeStamp(ts), _shipType(type), _direction(dir) { aliveIns++; }
		~Move() { aliveIns--; };

		long timeStamp() { return _timeStamp; }
		int shipType() { return _shipType; }
		char direction() { return _direction; }
	};

	std::list<Move> _moves;

public:
	PlayerMoves();
	~PlayerMoves();

	void addMove(long ts, int ship, char dir);


};

