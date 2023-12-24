#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "boat.h"

typedef struct {
    CaseType **grid;
    int size;
} GameBoard;

void initializeGameBoard(GameBoard *board, int size);
void freeGameBoard(GameBoard *board);
void printPlayerView(GameBoard *board);
void printComputerView(GameBoard *board);

#endif // GAMEBOARD_H
