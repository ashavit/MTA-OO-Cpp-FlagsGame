#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include "Board.h"
#include "Cell.h"
#include "Utils.h"

#define SET_BIT(NUM, I)             NUM |= ( 1 << (I) )
#define IS_BIT_I_SET(NUM, I)        ( (NUM >> I) & 1 )
#define MASK_WITH_N_LSBS_SET(N)     ( (1 << N) - 1 )

using namespace std;

#pragma mark - Life Cycle

int Board::aliveIns = 0;

Board::Board(UINT width, UINT height) : height(height), width(width) {
	aliveIns++;
    board = new Cell**[width];
    for (UINT col = 0; col < width; ++col) {
        board[col] = new Cell*[height];
        for (UINT row = 0; row < height; ++row) {
            board[col][row] = new Cell(row,col);
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

Board* Board::loadRandomBoard(Player& playerA, Player& playerB, UINT width, UINT height) {
    Board *b = new Board(width, height);
    
    int seas = BoardDensity::REGULAR;
    int forests = BoardDensity::REGULAR;
    
    b->randomPlaceSpecialCells(CellType::SEA, seas);
    b->randomPlaceSpecialCells(CellType::FORREST, forests);
    b->printBoard();
    
    b->getRandomCellInRows(1, 5)->setCellType(FLAG_A);
    b->getRandomCellInRows(9, 13)->setCellType(FLAG_B);
    
    // Init ships
    for (int type = ShipType::SHIP1; type <= ShipType::SHIP3; ++type )
    {
        Cell *c = b->getRandomCellInRows(1, 5);
        Ship *ship = new Ship(playerA, (ShipType)type, c);
        c->setStandingShip(ship);
    }
    
    for (int type = ShipType::SHIP7; type <= ShipType::SHIP9; ++type )
    {
        Cell *c = b->getRandomCellInRows(9, 13);
        Ship *ship = new Ship(playerB, (ShipType)type, c);
        c->setStandingShip(ship);
    }

    b->printBoard();
    
    return b;
}

Board* Board::loadBoardFromFile(Player& playerA, Player& playerB, const std::string& fileName, UINT width, UINT height) {
    Board *b = new Board(width, height);
    
    ifstream textfile(fileName); // default is text!
    if (!textfile.good())
    {
        /// TODO: Handle Error
    }
    
    list<string> errors;
    char validateToolsA = 0, validateToolsB = 0;
    bool isPlayerToolsValidA = true, isPlayerToolsValidB = true, isBoardValid = true;
    
    for (UINT row = 0; row < height && !textfile.eof(); ++row) {
        string buff;
        getline(textfile, buff);
        for (UINT col = 0; col < width; ++col) {
            switch (buff[col]) {
                case 'S':
                    b->board[col][row]->setCellType(CellType::SEA);
                    break;
                case 'T':
                    b->board[col][row]->setCellType(CellType::FORREST);
                    break;
                case 'A':
                    if ( IS_BIT_I_SET(validateToolsA, 0) ) { isPlayerToolsValidA = false; break; } // Make sure FlagA is set only once
                    b->board[col][row]->setCellType(CellType::FLAG_A);
                    SET_BIT(validateToolsA, 0);
                    break;
                case 'B':
                    if ( IS_BIT_I_SET(validateToolsB, 0) ) { isPlayerToolsValidB = false; break; } // Make sure FlagB is set only once
                    b->board[col][row]->setCellType(CellType::FLAG_B);
                    SET_BIT(validateToolsB, 0);
                    break;
                case '1':
                case '2':
                case '3':
                {
                    int bit = buff[col] - '1' + 1;
                    if ( IS_BIT_I_SET(validateToolsA, bit) ) { isPlayerToolsValidA = false; break; } // Make sure each ship is set only once
                    Ship *ship = new Ship(playerA, (ShipType)(buff[col] - '0'), b->board[col][row], true);
                    b->board[col][row]->setStandingShip(ship);
                    SET_BIT(validateToolsA, bit);
                    break;
                }
                    
                case '7':
                case '8':
                case '9':
                {
                    int bit = buff[col] - '7' + 1;
                    if ( IS_BIT_I_SET(validateToolsB, bit) ) { isPlayerToolsValidB = false; break; } // Make sure each ship is set only once
                    Ship *ship = new Ship(playerB, (ShipType)(buff[col] - '0'), b->board[col][row], true);
                    b->board[col][row]->setStandingShip(ship);
                    SET_BIT(validateToolsB, bit);
                    break;
                }
                    
                default:
                {
                    /// TODO: Handle unknown char - add error after making sure it wan't printed first
                    break;
                }
            }
        }
    }
    
    textfile.close();

    if ( !isPlayerToolsValidB || validateToolsB != MASK_WITH_N_LSBS_SET(4) ) {
        /// TODO: Ask Amir if there should be a space before file name
        errors.push_front("Wrong settings for player B tools in file" + fileName);
        isBoardValid = false;
    }
    if ( !isPlayerToolsValidA || validateToolsA != MASK_WITH_N_LSBS_SET(4) ) {
        /// TODO: Ask Amir if there should be a space before file name
        errors.push_front("Wrong settings for player A tools in file" + fileName);
        isBoardValid = false;
    }
    
    /// TODO: Return errors if found
    
    b->printBoard();
    
    return b;
}

#pragma mark - Public Functions

Cell* Board::getRandomCellInRows(size_t from, size_t to) {
    Cell *ptrRes = nullptr;
    Cell *ptrTemp = nullptr;
    size_t range = to - from + 1;
    
    do {
        int r = (int)((rand() % range) + from - 1);
        int c = rand() % width;
        ptrTemp = board[c][r];
        if (ptrTemp->getCellType() == CellType::REGULAR && ptrTemp->getStandingShip() == nullptr) {
            ptrRes = ptrTemp;;
        }
    } while (ptrRes == nullptr);
    return ptrRes;
}

Cell* Board::getNextCell(const Cell* cell, Direction direction) {
    int r, c;
    r = cell->row;
    c = cell->column;
    
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

#pragma mark Outputs

void Board::drawBoard() {
    Cell* cell;
	clearScreen();
	for (int column = 1; column <= width; ++column) {
		gotoxy(column, 0);
		std::cout << (char)('A' + column - 1);
	}
	for (int row = 1; row <= height; ++row) {
		gotoxy(0, row);
		std::cout << row;
	}
	std::cout.flush();

	for (int column = 0; column < width; ++column) {
		for (int row = 0; row < height; ++row) {
			cell = board[column][row];
			drawCell(cell);
		}
    }
}

void Board::drawCell(Cell* cell) {
	gotoxy(cell->column + 1, cell->row + 1);

	if (cell->getStandingShip() != nullptr && cell->getStandingShip()->alive()) {
		setTextColor(BLACK, GREY);
		cout << cell->getStandingShip()->type();
	}
	else {
		switch (cell->getCellType()) {
        case CellType::SEA:
			setTextColor(YELLOW, BLUE);
			cout << 'S';
			break;
		case CellType::FORREST:
			setTextColor(BLUE, GREEN);
			cout << 'F';
			break;
		case CellType::FLAG_A:
			setTextColor(BLACK, RED);
			cout << 'A';
			break;
		case CellType::FLAG_B:
			setTextColor(BLACK, YELLOW);
			cout << 'B';
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

void Board::printBoard()
{
    for (UINT row = 0; row < height; ++row) {
        cout << "  ";
        for (UINT col = 0; col < width; ++col) {
            Cell *c = board[col][row];
            if (c->getStandingShip() != nullptr) {
                cout << (int)c->getStandingShip()->type();
            }
            else {
                switch (c->getCellType()) {
                    case FORREST:
                        cout << "F";
                        break;
                    case SEA:
                        cout << "S";
                        break;
                    case FLAG_A:
                        cout << "A";
                        break;
                    case FLAG_B:
                        cout << "B";
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

void Board::printMessage(const string message, bool onFullScreen, bool waitForResponse) {
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
		char c;
		cout << "Press any key to continue..." << endl;
		cin >> c;
	}
}

int Board::getPlayerStatsLocation() {
	return width + 30;
}

void Board::saveToFile(std::string name) {
    string fileName = newFileName(name);
    ofstream textfile(fileName); // default is text!
    if (!textfile.good())
    {
        /// TODO: Handle Error
    }
    
    for (UINT row = 0; row < height; ++row) {
        for (UINT col = 0; col < width; ++col) {
            Cell *c = board[col][row];
            if (c->getStandingShip() != nullptr) {
                textfile << (int)c->getStandingShip()->type();
            }
            else {
                switch (c->getCellType()) {
                    case FORREST:
                        textfile << "T";
                        break;
                    case SEA:
                        textfile << "S";
                        break;
                    case FLAG_A:
                        textfile << "A";
                        break;
                    case FLAG_B:
                        textfile << "B";
                        break;
                        
                    default:
                        textfile << " ";
                        break;
                }
            }
        }
        textfile << endl;
    }
    
    textfile.close();
}

#pragma mark - Private Functions

void Board::randomPlaceSpecialCells(CellType type, int count) {
    for (int i = 0; i < count; ++i) {
        getRandomCellInRows(1, height)->setCellType(type);
    }
}

string Board::newFileName(string format) {
    string fileName = format + ".gboard";
    if (std::ifstream(fileName)) // File already exists
    {
        return newFileName(format + "-new");
    }
    else // File does not exist
    {
        return fileName;
    }
}

