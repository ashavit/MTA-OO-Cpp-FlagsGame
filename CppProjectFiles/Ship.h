//
//  Ship.h
//  Cpp-FlagsProject
//
//  Created by Amir Shavit on 26/03/2017.
//  Copyright © 2017 Amir Shavit. All rights reserved.
//

#ifndef Ship_h
#define Ship_h

#include <stdio.h>

/// TODO: Amir: Include correct cell class when available
class Cell;

enum ShipType {SHIP1 = 1, SHIP2, SHIP3, SHIP7 = 7, SHIP8, SHIP9 };

class Ship {
    
    const ShipType shipType;
    Cell* initialCell;
    Cell* currentCell;
    bool isAlive = true;
    
public:
    Ship(ShipType type, Cell* startingCell);
    
    Cell* cell();
    bool alive();
    ShipType getShipType();

    bool canMoveToCell(Cell& cell);
    void resetToInitialState();

    bool didFindFlag();
};

#endif /* Ship_h */
