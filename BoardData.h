#pragma once

class BoardData {
public:
	enum { rows = 13, cols = 13 };
	virtual ~BoardData() { }
	/* Coordinates start from 1,1 */
	virtual char charAt(int x, int y) const = 0;
};