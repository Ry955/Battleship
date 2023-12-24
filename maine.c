#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

int main() {
    // Initialization of the random number for next
    srand(time(NULL));

    // Create and initialize the game
    Game *game = initializeGame();
    if (game == NULL) {
        fprintf(stderr, "Failed to initialize game.\n");
        return EXIT_FAILURE;
    }

    bool playerTurnFlag = true; // True if it's the player's turn, false for the computer.
    bool gameIsOver = false;

    // Game's loop
    while (!gameIsOver) {
        if (playerTurnFlag) {
            // Player's turn
            printf("Player's Turn:\n");
            playerTurn(&game->player2Board); // The player shoots at the computer board
            printf("\nYour Board:\n\n");
            printPlayerView(&game->player1Board);
            printf("Opponent's Board:\n");
            printComputerView(&game->player2Board);

        } else {
            // Computer's turn
            printf("Computer's Turn:\n");
            computerTurn(&game->player1Board); // The computer shoots at the player’s board
            printf("Your Board:\n");
            printPlayerView(&game->player1Board); // Player(s board)
            printf("\nOpponent's Board:\n\n");// Computer's view, we can see where we did shoot
            printComputerView(&game->player2Board);

        }

        // Check if the game is over
        if (isGameOver(&game->player1Board)) {
            announceWinner(false); // Computer wins
            gameIsOver = true;
        } else if (isGameOver(&game->player2Board)) {
            announceWinner(true); // Player wins
            gameIsOver = true;
        }

        // Change turn
        playerTurnFlag = !playerTurnFlag;
    }

    // At the end of the game, release all allocated data.
    freeGame(game);

    return EXIT_SUCCESS;
}
