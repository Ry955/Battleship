#include <stdio.h>
#include <stdlib.h>
#include "gameboard.h"

void initializeGameBoard(GameBoard *board, int size) {
    board->size = size;
    board->grid = (CaseType**)malloc(sizeof(CaseType*) * size);
    for (int i = 0; i < size; i++) {
        board->grid[i] = (CaseType*)malloc(sizeof(CaseType) * size);
        for (int j = 0; j < size; j++) {
            board->grid[i][j] = WATER; // Initializes each case to WATER.
        }
    }
}

void freeGameBoard(GameBoard *board) {
    if (board && board->grid) {
        for (int i = 0; i < board->size; i++) {
            free(board->grid[i]);  // Free each line of the table.
        }
        free(board->grid);  // Clear the table of lines
    }
}

void printPlayerView(GameBoard *board) {
    if (board == NULL || board->grid == NULL) {
        printf("Invalid game board.\n");
        return;
    }

    // Header for the columns
    printf("  ");
    for (int x = 0; x < board->size; x++) {
        printf("%d ", x);
    }
    printf("\n");

    // Browse the gameboard and display the status of each box
    for (int y = 0; y < board->size; y++) {
        // Line number on the left
        printf("%d ", y);

        for (int x = 0; x < board->size; x++) {
            switch (board->grid[y][x]) {
                case WATER:
                    printf("~ "); // Water not targeted
                    break;
                case WATER_SHOT:
                    printf("o "); // Water targeted by a shot
                    break;
                case BOAT:
                    printf("B "); // Part of a boat not touched
                    break;
                case WRECK:
                    printf("X "); // Part of a boat hit
                    break;
                default:
                    printf("? "); // Unknown
            }
        }
        printf("\n");
    }
}

void printComputerView(GameBoard *board) {
    if (board == NULL || board->grid == NULL) {
        printf("Invalid game board.\n");
        return;
    }

    // En-tête pour les colonnes.
    printf("  ");
    for (int x = 0; x < board->size; x++) {
        printf("%d ", x);
    }
    printf("\n");

    // Parcourir le plateau et afficher l'état de chaque case.
    for (int y = 0; y < board->size; y++) {
        // Numéro de la ligne à gauche.
        printf("%d ", y);

        for (int x = 0; x < board->size; x++) {
            switch (board->grid[y][x]) {
                case WATER_SHOT:
                    printf("o "); // Eau ciblée par un tir.
                    break;
                case WRECK:
                    printf("X "); // Partie d'un bateau touchée.
                    break;
                case WATER: // Cache les cases d'eau et de bateaux non touchés.
                case BOAT:
                default:
                    printf("~ "); // Affiche comme de l'eau non ciblée.
            }
        }
        printf("\n");
    }
}
