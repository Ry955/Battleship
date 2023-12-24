#include <stdlib.h>
#include <stdbool.h>
#include "boat.h"


Boat *createBoat(int size, int x, int y, Orientation orientation) {
    // Allocate memory for a new Boat.
    Boat *newBoat = (Boat*)malloc(sizeof(Boat));
    if (newBoat == NULL) {
        fprintf(stderr, "Memory allocation failed for new boat.\n");
        return NULL;
    }

    // Initialize the boat's properties.
    newBoat->size = size;
    newBoat->x = x;
    newBoat->y = y;
    newBoat->orientation = orientation;

    return newBoat; // Return the pointer to the new boat.
}

// Function to check if a boat is still alive
bool isBoatAlive(Boat *boat) {
    if (boat == NULL) {
        return false; // Return false if boat pointer is NULL.
    }

    // Check if the number of hits is less than the size of the boat.
    return boat->hit_count < boat->size;
}


bool canPlaceBoat(GameBoard *board, Boat *boat) {
    // Check if the boat leaves the board
    if (boat->orientation == HORIZONTAL) {
        if (boat->x + boat->size > board->size) return false;
    } else { // VERTICAL
        if (boat->y + boat->size > board->size) return false;
    }

    // Check if the boat touches or overlaps another boat
    for (int i = 0; i < boat->size; i++) {
        int x = boat->x + (boat->orientation == HORIZONTAL ? i : 0);
        int y = boat->y + (boat->orientation == VERTICAL ? i : 0);

        // Check the actual case
        if (board->grid[y][x] != WATER) return false;

        // Check the boxes around to avoid touching another boat.
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < board->size && ny >= 0 && ny < board->size) {
                    if (board->grid[ny][nx] != WATER) return false;
                }
            }
        }
    }
    return true;
}

void setBoatOnBoard(GameBoard *board, Boat *boat) {
    for (int i = 0; i < boat->size; i++) {
        int x = boat->x + (boat->orientation == HORIZONTAL ? i : 0);
        int y = boat->y + (boat->orientation == VERTICAL ? i : 0);

        board->grid[y][x] = BOAT; // Place a part of the boat on the game board
    }
}

// Function to place a boat on the game board randomly

bool placeRandomBoat(GameBoard *board, Boat *boat) {
    if (!board || !boat) return false; // Check if pointer no-NULL.

    // Set a maximum number of attempts to avoid an infinite loop if placement is not possible.
    int maxTries = 100;
    for (int tries = 0; tries < maxTries; tries++) {
        // Generate random position and orientation
        boat->x = rand() % board->size;
        boat->y = rand() % board->size;
        boat->orientation = (rand() % 2 == 0) ? HORIZONTAL : VERTICAL;

        // Check if the boat can be placed
        if (canPlaceBoat(board, boat)) {
            setBoatOnBoard(board, boat); // Place the boat
            return true; // Success
        }
    }
    return false; // Fail after the maxTries
  }
