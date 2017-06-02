#pragma once

struct GameMove
{
	const int from_x, from_y;
	const int to_x, to_y;
	GameMove(int x1, int y1, int x2, int y2) : from_x(x1), from_y(y1), to_x(x2), to_y(y2) { }
};
