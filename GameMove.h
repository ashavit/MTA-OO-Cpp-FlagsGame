#pragma once

// new and improved "GameMove"
struct GameMove {
	const int from_x, from_y;
	const int to_x, to_y;
	GameMove(int x1, int y1, int x2, int y2) : from_x(x1), from_y(y1), to_x(x2), to_y(y2) { }
	GameMove& operator=(const GameMove& gm) {
		const_cast<int&>(from_x) = gm.from_x;
		const_cast<int&>(from_y) = gm.from_y;
		const_cast<int&>(to_x) = gm.to_x;
		const_cast<int&>(to_y) = gm.to_y;
		return *this;
	}
};
