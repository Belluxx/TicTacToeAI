#ifndef TICTACTOEAI_BOARD_H
#define TICTACTOEAI_BOARD_H

#include <random>

typedef struct {
    unsigned short int x;
    unsigned short int y;
} pos;

class Board {
private:
    char **boardData;
    int movesCount;
    std::uniform_real_distribution<float> rndFloatGenerator;
    std::uniform_int_distribution<int> rndIntGenerator;
    std::mt19937 rndEngine;

    void nextTurn();

    void clearBoard();

public:
    static const char X = 'X';
    static const char O = 'O';
    static const char NONE = '.';
    int l;
    int winStr;
    char turn;
    int cellsCount;

    Board(int, int);

    void reset();

    void print();

    int performAction(char, pos);

    int getGameStatus();

    std::string getStateHash();

    std::string getStateHash(char tag, pos p);

    int getAvailableActions(pos *);

    static void printPos(pos);

    float randomUnitFloat();

    int randomInt(int);
};


#endif //TICTACTOEAI_BOARD_H
