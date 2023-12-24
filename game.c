#include <stdlib.h>
#include "game.h"


Game *initializeGame() {
    Game *newGame = (Game*)malloc(sizeof(Game));
    if (newGame == NULL) {
        fprintf(stderr, "Memory allocation failed for new game.\n");
        return NULL;
    }

    // Initialize the game board for the two players
    initializeGameBoard(&newGame->player1Board, BOARD_SIZE);
    initializeGameBoard(&newGame->player2Board, BOARD_SIZE);

    srand(time(NULL)); // Initialization to use random number

    // Initialize and place boat for each player
    for (int i = 0; i < MAX_BOATS; i++) {
        newGame->player1Boats[i] = *createBoat(rand() % 3 + 2, 0, 0, HORIZONTAL); 
        if (!placeRandomBoat(&newGame->player1Board, &newGame->player1Boats[i])) {
            fprintf(stderr, "Failed to place a boat for player 1.\n");
        }

        newGame->player2Boats[i] = *createBoat(rand() % 3 + 2, 0, 0, HORIZONTAL);
        if (!placeRandomBoat(&newGame->player2Board, &newGame->player2Boats[i])) {
            fprintf(stderr, "Failed to place a boat for player 2.\n");
        }
    }

    return newGame; // Return a pointer towards the new game
}

void freeGame(Game *game) {
    if (game) {
        // Free the two gameboard
        freeGameBoard(&game->player1Board);
        freeGameBoard(&game->player2Board);

        free(game);
    }
}
