#include <string>
#include <iostream>
#include <list>
#include "Commons.h"
#include "Board.h"
#include "Cell.h"
#include "Utils.h"

using namespace std;

//*********** Life Cycle ***********//

int Board::aliveIns = 0;

Board::Board(UINT width, UINT height) : height(height), width(width) {
	aliveIns++;
	board = new Cell**[width];
	for (UINT col = 0; col < width; ++col) {
		board[col] = new Cell*[height];
		for (UINT row = 0; row < height; ++row) {
			board[col][row] = new Cell(row, col);
		}
	}
}

Board::~Board() {
	aliveIns--;
	for (UINT col = 0; col < width; ++col) {
		for (UINT row = 0; row < height; ++row) {
			delete board[col][row];
		}
		delete board[col];
	}
	delete board;
}

//*********** Public functions ***********//

Cell* Board::getRandomCellInRows(size_t from, size_t to) const {
	Cell* ptrRes = nullptr;
	Cell* ptrTemp = nullptr;
	size_t range = to - from + 1;

	do {
		int r = static_cast<int>((rand() % range) + from - 1);
		int c = rand() % width;
		ptrTemp = board[c][r];
		if (ptrTemp->getCellType() == CellType::REGULAR && ptrTemp->getStandingShip() == nullptr) {
			ptrRes = ptrTemp;;
		}
	}
	while (ptrRes == nullptr);
	return ptrRes;
}

Cell* Board::getNextCell(const Cell* cell, Direction direction) const {
	int r = cell->row;
	int c = cell->column;

	switch (direction) {
	case Direction::UP:
		r--;
		break;
	case Direction::DOWN:
		r++;
		break;
	case Direction::LEFT:
		c--;
		break;
	case Direction::RIGHT:
		c++;
		break;
	default:
		break;
	}

	if (c >= 0 && c < width &&
		r >= 0 && r < height) {
		return board[c][r];
	}
	return nullptr;
}

//*********** Outputs ***********//

void Board::drawBoard() const {
	clearScreen();
	for (int column = 1; column <= width; ++column) {
		gotoxy(column, 0);
		std::cout << static_cast<char>('A' + column - 1);
	}
	for (int row = 1; row <= height; ++row) {
		gotoxy(0, row);
		std::cout << row;
	}
	std::cout.flush();

	for (int column = 0; column < width; ++column) {
		for (int row = 0; row < height; ++row) {
			Cell * cell = board[column][row];
			drawCell(cell);
		}
	}
}

void Board::drawCell(Cell* cell) const {
	gotoxy(cell->column + 1, cell->row + 1);

	if (cell->getStandingShip() != nullptr && cell->getStandingShip()->alive()) {
		setTextColor(BLACK, GREY);
		cout << cell->getStandingShip()->type();
	}
	else {
		switch (cell->getCellType()) {
		case CellType::SEA:
			setTextColor(YELLOW, BLUE);
			cout << BOARD_MARK_SEA;
			break;
		case CellType::FORREST:
			setTextColor(BLUE, GREEN);
			cout << BOARD_MARK_FOREST;
			break;
		case CellType::FLAG_A:
			setTextColor(BLACK, RED);
			cout << BOARD_MARK_FLAG_A;
			break;
		case CellType::FLAG_B:
			setTextColor(BLACK, YELLOW);
			cout << BOARD_MARK_FLAG_B;
			break;
		case CellType::REGULAR:
		default:
			setTextColor(WHITE, WHITE);
			cout << ' ';
			break;
		}
	}
	cout.flush();
}

void Board::printBoard() const {
	if (DEBUG) {
		for (UINT row = 0; row < height; ++row) {
			cout << "  ";
			for (UINT col = 0; col < width; ++col) {
				Cell* c = board[col][row];
				if (c->getStandingShip() != nullptr) {
					cout << static_cast<int>(c->getStandingShip()->type());
				}
				else {
					switch (c->getCellType()) {
					case FORREST:
						cout << BOARD_MARK_FOREST;
						break;
					case SEA:
						cout << BOARD_MARK_SEA;
						break;
					case FLAG_A:
						cout << BOARD_MARK_FLAG_A;
						break;
					case FLAG_B:
						cout << BOARD_MARK_FLAG_B;
						break;

					default:
						cout << '-';
						break;
					}
				}
				cout << "  ";
			}
			cout << endl;
		}
		cout << endl << endl;
	}
}

void Board::printMessage(const string message, bool onFullScreen, bool waitForResponse) const {
	setTextColor(WHITE);
	if (onFullScreen) {
		clearScreen();
		gotoxy(20, 15);
	}
	else {
		gotoxy(10, height + 5);
	}
	cout << message << endl;

	if (waitForResponse) {
		waitForAnyKeyToContinue();
	}
}

int Board::getPlayerStatsLocation() const {
	return width + 30;
}

//*********** Private functions ***********//

void Board::randomPlaceSpecialCells(CellType type, int count) const {
	for (int i = 0; i < count; ++i) {
		getRandomCellInRows(1, height)->setCellType(type);
	}
}
