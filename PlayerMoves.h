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

	public:
		Move(int type, char dir) : _shipType(type), _direction(dir) { aliveIns++; }
		~Move() { aliveIns--; };

		int shipType() const { return _shipType; }
		Direction direction() const;
	};

private:
	static int aliveIns;

	std::map<unsigned long, Move*> _moves;

public:
	PlayerMoves();
	~PlayerMoves();

	void addMove(unsigned long ts, int ship, char dir);
	void addMove(unsigned long ts, Ship& ship, Direction dir);
	const Move* const getMove(unsigned long timeStamp);

	unsigned int moveCount() { return _moves.size(); }
	bool isEnded(unsigned long ts);

};

