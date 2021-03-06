#pragma once

#include <iostream>
#include <map>

class Ship;
enum class Direction;

class PlayerMoves
{
public:
	class Move
	{
		static int aliveIns;

		int _shipType;
		char _direction;

		friend std::ostream& operator<<(std::ostream& out, const Move& move);

	public:
		Move(int type, char dir) : _shipType(type), _direction(dir) { aliveIns++; }
		~Move() { aliveIns--; };
		Move(Move const&) = delete;
		void operator=(Move const&) = delete;
		static int aliveInstances() { return aliveIns; }

		int shipType() const { return _shipType; }
		Direction direction() const;
		bool isMoveValid() const;
	};

private:
	static int aliveIns;

	std::map<unsigned long, Move*> _moves;

public:
	PlayerMoves();
	~PlayerMoves();
	PlayerMoves(PlayerMoves const&) = delete;
	void operator=(PlayerMoves const&) = delete;
	static int aliveInstances() { return aliveIns; }

	void addMove(unsigned long ts, int ship, char dir);
	void addMove(unsigned long ts, Ship& ship, Direction dir);
	const Move* getMove(unsigned long timeStamp);

	std::map<unsigned long, Move*> getMovesList() const { return _moves; }
	unsigned int moveCount() const { return _moves.size(); }
	bool isEnded(unsigned long ts) const;

	static Direction directionFromChar(char _direction);
	static char charFromDirection(Direction _direction);
};

