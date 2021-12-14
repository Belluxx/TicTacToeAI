#ifndef TICTACTOEAI_AGENT_H
#define TICTACTOEAI_AGENT_H

#include <map>
#include "../board/Board.h"

class Agent {
private:
    Board *board;
    float expRate;
    float decayGamma;
    float learningRate;
    std::string *gameStates;
    int gameStatesSize;
    std::map<std::string, float> svPairs;

public:
    char tag;

    explicit Agent(Board *, char = Board::NONE, float = 0.3, float = 0.9, float = 0.2);

    ~Agent();

    void feedReward(float);

    void newGame();

    void addGameState(std::string state);

    void debug(bool full = false);

    void save(const std::string &fileName);

    void load(const std::string &fileName);

    pos chooseAction(bool = false, bool = false);
};


#endif //TICTACTOEAI_AGENT_H
