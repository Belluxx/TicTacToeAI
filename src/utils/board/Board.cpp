#include "Board.h"

/**
 * Create a new board
 * @param l Size of the board
 * @param winStr Consecutive symbols needed to win
 */
Board::Board(int l, int winStr) {
    this->l = l;
    this->cellsCount = l * l;
    this->winStr = winStr;
    this->movesCount = 0;

    boardData = new char *[l];
    for (int i = 0; i < l; i++) {
        boardData[i] = new char[l];
    }

    turn = X;

    std::random_device rndDevice;
    this->rndEngine = std::mt19937(rndDevice());
    this->rndFloatGenerator = std::uniform_real_distribution<float>(0.0f, 1.0f);
    this->rndIntGenerator = std::uniform_int_distribution<int>(0, 32767);

    this->reset();
}

/**
 * Get the state of the game
 * @return 0 for normal, 1 for X victory, 2 for
 * O victory, 3 for draw
 */
int Board::getGameStatus() {
    // Cerca vittorie orizzontali
    for (int y = 0; y < l; y++) {
        int xs = 0, os = 0;
        for (int x = 0; x < l; x++) {
            if (boardData[y][x] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[y][x] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
        }
    }

    // Cerca vittorie verticali
    for (int x = 0; x < l; x++) {
        int xs = 0, os = 0;
        for (int y = 0; y < l; y++) {
            if (boardData[y][x] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[y][x] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
        }
    }

    // Cerca vittorie diagonali avanti inferiori + centrale
    for (int y = 0; y < l; y++) {
        int tx = 0, xs = 0, os = 0;
        for (int ty = y; ty < l; ty++) {
            if (boardData[ty][tx] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[ty][tx] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
            tx++;
        }
    }

    // Cerca vittorie diagonali avanti superiori
    for (int y = 1; y < l; y++) {
        int tx = 0, xs = 0, os = 0;
        for (int ty = y; ty < l; ty++) {
            if (boardData[tx][ty] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[tx][ty] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
            tx++;
        }
    }

    // Cerca vittorie diagonali indietro inferiori + centrale
    for (int x = l - 1; x >= 0; x--) {
        int tx = x, xs = 0, os = 0;
        for (int ty = 0; ty <= x; ty++) {
            if (boardData[tx][ty] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[tx][ty] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
            tx--;
        }
    }

    // Cerca vittorie diagonali indietro superiori
    for (int y = 1; y < l; y++) {
        int ty = y, xs = 0, os = 0;
        for (int x = l - 1; x >= y; x--) {
            if (boardData[ty][x] == X) xs++;
            else xs = 0;
            if (xs >= winStr) return 1;

            if (boardData[ty][x] == O) os++;
            else os = 0;
            if (os >= winStr) return 2;
            ty++;
        }
    }

    if (movesCount >= cellsCount) return 3;

    return 0;
}

/**
 * Perform an action on the board
 * @param tag tag of who performs the action
 * @param p position of the action
 * @return 0 if the action is allowed, 1 if it is not allowed
 */
int Board::performAction(char tag, pos p) {
    if ((p.x >= 0 && p.x < l && p.y >= 0 && p.y < l) && (boardData[p.y][p.x] == NONE)) {
        boardData[p.y][p.x] = tag;
        nextTurn();
    } else return 0;

    return 1;
}

/**
 * Find all playable actions and put the corresponding positions
 * in the array passed as parameter.
 * @param validActions array of size board.l^2 where actions
 * positions will be put.
 * @return the number of playable actions, or size of the array.
 */
int Board::getAvailableActions(pos *validActions) {
    int actionsCount = 0;
    for (int y = 0; y < l; y++)
        for (int x = 0; x < l; x++)
            if (boardData[y][x] == NONE) {
                validActions[actionsCount].x = x;
                validActions[actionsCount].y = y;
                actionsCount++;
            }

    return actionsCount;
}

/**
 * Print the board with coordinates
 */
void Board::print() {
    printf("  0 ");
    for (int x = 1; x < l; x++)
        printf("%d ", x);
    printf("\n");

    for (int y = 0; y < l; y++) {
        printf("%d", y);
        for (int x = 0; x < l; x++) {
            printf(" %c", boardData[y][x]);
        }
        printf("\n");
    }
}

/**
 * Pass to the next turn
 */
void Board::nextTurn() {
    turn = turn == X ? O : X;
    movesCount++;
}

/**
 * Flattens the board matrix to a string.
 * @return string that identifies the current state.
 */
std::string Board::getStateHash() {
    std::string hash;
    int y = 0;
    for (int i = 0; i < cellsCount; i++) {
        hash += boardData[y][i % l];
        if (i % l == l - 1) y++;
    }

    return hash;
}

/**
 * Flattens the board matrix to a string after performing
 * the action.
 * @param tag tag of who performs the action
 * @param p position of the action
 * @return string that identifies the future state.
 */
std::string Board::getStateHash(char tag, pos p) {
    std::string hash;
    int y = 0, x;
    for (int i = 0; i < cellsCount; i++) {
        x = i % l;

        if (x == p.x && y == p.y) hash += tag;
        else hash += (boardData[y][x]);

        if (i % l == l - 1) y++;
    }

    return hash;
}

/**
 * Clear the board
 */
void Board::clearBoard() {
    for (int y = 0; y < l; y++) {
        for (int x = 0; x < l; x++) {
            this->boardData[y][x] = NONE;
        }
    }
}

/**
 * Print a position
 * @param p The position
 */
void Board::printPos(pos p) {
    printf("(%d, %d)\n", p.x, p.y);
}

/**
 * Reset and clear the board
 */
void Board::reset() {
    movesCount = 0;
    turn = X;
    clearBoard();
}

/**
 * Generate random float between 0.0 and 1.0 excluded.
 * @return random float between 0.0 and 1.1 excluded
 */
float Board::randomUnitFloat() {
    return rndFloatGenerator(rndEngine);
}

/**
 * Generate random int between 0 and max excluded.
 * @param max maximum number
 * @return random int between 0 and max excluded
 */
int Board::randomInt(int max) {
    return rndIntGenerator(rndEngine) % max;
}