#ifndef TICTACTOEAI_BOARDMANAGER_H
#define TICTACTOEAI_BOARDMANAGER_H

#include "Board.h"
#include "../ai/Agent.h"

class BoardManager {
private:
    static void delay(int);

public:
    Board board = Board(0, 0);

    BoardManager();

    void makeBoard(const std::string &);

    void AiVsHuman(const std::string &, bool);

    void AiVsAi(const std::string &, const std::string &);

    void makeBoard(int, int);

    void train(int, int, int);
};


#endif //TICTACTOEAI_BOARDMANAGER_H
