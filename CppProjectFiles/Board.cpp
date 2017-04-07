#include "Board.h"
#include "Cell.h"
#include "Utils.h"

using namespace std;

#pragma mark - Life Cycle

Board::Board(size_t height, size_t width) : height(height), width(width) {
    board = new Cell**[height];
    for (size_t i = 0; i < height; ++i) {
        board[i] = new Cell*[width];
        for (size_t j = 0; j < width; ++j) {
            board[i][j] = new Cell(i,j);
        }
    }
}

Board::~Board() {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            delete board[i][j];
        }
        delete board[i];
    }
    delete board;
}

Board* Board::loadRandomBoard(size_t height, size_t width) {
    Board *b = new Board(height, width);
    
    /// TODO: Move this to param or const
    int seas = 20;
    int forests = 20;
    
    b->randomPlaceSpecialCells(CellType::SEA, seas);
    b->randomPlaceSpecialCells(CellType::FORREST, forests);
    b->printBoard();
    
    b->getRandomCellInRows(1, 5)->setCellType(FLAG_A);
    b->getRandomCellInRows(9, 13)->setCellType(FLAG_B);
    b->printBoard();
    
    return b;
}

Cell* Board::getRandomCellInRows(size_t from, size_t to) {
    Cell *ptrRes = nullptr;
    Cell *ptrTemp = nullptr;
    size_t range = to - from + 1;
    
    do {
        int r = (int)((rand() % range) + from - 1);
        int c = rand() % width;
        ptrTemp = board[r][c];
        if (ptrTemp->getCellType() == CellType::REGULAR && ptrTemp->getStandingShip() == nullptr) {
            ptrRes = ptrTemp;;
        }
    } while (ptrRes == nullptr);
    return ptrRes;
}

void Board::draw() {
    Cell* cell;
    Ship* standingShip;
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            cell = board[row][column];
            standingShip = cell->getStandingShip();
            switch (cell->getCellType()){
                case SEA:
                    //setTextColor(WHITE, BLUE);
                    break;
                case FORREST:
                    //setTextColor(WHITE, GREEN);
                    break;
                case FLAG_A:
                    //setTextColor(WHITE, RED);
                    break;
                case FLAG_B:
                    //setTextColor(WHITE, YELLOW);
                    break;
                case REGULAR:
                default:
                    //setTextColor(WHITE, BLACK);
                    break;
            }
            std::cout << (standingShip != nullptr ? standingShip->type() : ' ');
        }
        std::cout << std::endl;
    }
}

void Board::randomPlaceSpecialCells(CellType type, int count) {
    for (int i = 0; i < count; ++i) {
        getRandomCellInRows(1, height)->setCellType(type);
    }
}

void Board::printBoard()
{
    for (size_t i = 0; i < height; ++i) {
        cout << "  ";
        for (size_t j = 0; j < width; ++j) {
            Cell *c = board[i][j];
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

