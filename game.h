#ifndef GAME_H
#define GAME_H

#include "gameboard.h"
#include "boat.h"

typedef struct {
    GameBoard player1Board;
    GameBoard player2Board;
    Boat player1Boats[MAX_BOATS];
    Boat player2Boats[MAX_BOATS];
} Game;

Game *initializeGame();
void freeGame(Game *game);

#endif // GAME_H
