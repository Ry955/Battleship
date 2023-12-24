#ifndef BOAT_H
#define BOAT_H

#include "gameboard.h"

typedef struct {
    int size;
    int x, y;
    Orientation orientation;
    int hit_count;
} Boat;

Boat *createBoat(int size, int x, int y, Orientation orientation);
bool isBoatAlive(Boat *boat);
bool canPlaceBoat(GameBoard *board, Boat *boat);
void setBoatOnBoard(GameBoard *board, Boat *boat);
bool placeRandomBoat(GameBoard *board, Boat *boat);

#endif // BOAT_H
