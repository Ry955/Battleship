#define MAX_BOATS 5       // Define the maximum number of boats for a player
#define BOARD_SIZE 10       // Define the size of the board

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>    // Include for bool type
#include <time.h>     // rand() and srand() utilisation


typedef enum {
    WATER,      // Water case
    WATER_SHOT, // Water case where a shot has been made
    BOAT,       // Case where a boat's part has not been hitten yet
    WRECK,      // Case where a boat's part has not been hitten yet
} CaseType;


typedef enum {
    HORIZONTAL, // Indicates the boat is placed horizontally.
    VERTICAL,   // Indicates the boat is placed vertically.
} Orientation;

typedef struct{
    int size;             // The length of the boat in terms of the number of grid squares it occupies.
    int x, y;             // Coordinates of the reference position of the boat (e.g., the top-left position).
    Orientation orientation; // The orientation of the boat: HORIZONTAL or VERTICAL.
    int hit_count;         // The number of hits the boat has received.
} Boat;

typedef struct {
    CaseType **grid;  // Pointer to a 2D CaseType table representing the tray.
    int size;         // Size of one side of the square matrix 
} GameBoard;

typedef struct {
    GameBoard player1Board;            // Game board for player 1.
    GameBoard player2Board;            // Game board for player 2.
    Boat player1Boats[MAX_BOATS];      // Array of boats for player 1.
    Boat player2Boats[MAX_BOATS];      // Array of boats for player 2.
} Game;



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

// Function to display a board with the status of the boats with player view
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

// Function to make a shot on a given square of the board
void shootAt(GameBoard *board, int x, int y) {
    if (x >= 0 && x < board->size && y >= 0 && y < board->size) {
        // Check the current status of the case and make the changes
        switch (board->grid[y][x]) {
            case WATER:
                board->grid[y][x] = WATER_SHOT;
                printf("Missed! You hit the water.\n");
                break;
            case BOAT:
                board->grid[y][x] = WRECK;
                printf("Hit! You've touched a boat.\n");
                break;
            case WATER_SHOT:
            case WRECK:
                printf("This location has already been targeted.\n");
                break;
        }
    } else {
        printf("Invalid coordinates. Choose within 0 and %d.\n", board->size - 1);
    }
}

// Function that sequences a game round for the player
void playerTurn(GameBoard *enemyBoard) {
    int x, y;

    printf("Enter coordinates for your shot (X Y): ");
    scanf("%d %d", &x, &y);  // Ask the player to enter coordinates

    shootAt(enemyBoard, x, y);  // Shoot and make the changes if there is
}



// Function to check if a box has already been targeted
bool isAlreadyTargeted(GameBoard *board, int x, int y) {
    return board->grid[y][x] == WATER_SHOT || board->grid[y][x] == WRECK;
}

// Function that sequences a game round for the player
void computerTurn(GameBoard *playerBoard) {
    int x, y;
    bool shotValid = false;

    // Continue to generate random coordinates until an untargeted box is found
    while (!shotValid) {
        x = rand() % playerBoard->size;
        y = rand() % playerBoard->size;

        if (!isAlreadyTargeted(playerBoard, x, y)) {
            shotValid = true;
            shootAt(playerBoard, x, y);  // Shoot and make the changes
            printf("Computer shot at (%d, %d).\n", x, y);
        }
    }
}


bool isGameOver(GameBoard *board) {
    for (int y = 0; y < board->size; y++) {
        for (int x = 0; x < board->size; x++) {
            if (board->grid[y][x] == BOAT) { // If an undamaged boat is found.
                return false;
            }
        }
    }
    return true; // All boats have been touched
}

void announceWinner(bool playerWon) {
    if (playerWon) {
        printf("You've won, you're a true Amiral !\n");
    } else {
        printf("Computer wins. It doesn't matter, sailor\n");
    }
}


void freeGame(Game *game) {
    if (game) {
        // Free the two gameboard
        freeGameBoard(&game->player1Board);
        freeGameBoard(&game->player2Board);

        free(game);
    }
}

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