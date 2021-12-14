#include <iostream>
#include "Agent.h"

/**
 * An autonomous agent capable of training and playing
 * against humans or other autonomous entities.
 * @param expRate Exploration rate
 * @param decayGamma Value of future reward
 * @param learningRate Learning rate
 */
Agent::Agent(Board *board, char tag, float expRate, float decayGamma, float learningRate) {
    this->board = board;
    this->tag = tag;
    this->expRate = expRate;
    this->decayGamma = decayGamma;
    this->learningRate = learningRate;

    this->gameStates = new std::string[board->cellsCount];
    this->gameStatesSize = 0;
    this->svPairs = {};
}

pos Agent::chooseAction(bool fightMode, bool debugMode) {
    pos action;
    pos *availableActions = new pos[board->cellsCount];
    int availableActionsCount = board->getAvailableActions(availableActions);
    if (availableActionsCount == 0) {
        throw std::out_of_range("No actions available");
    }

    if (!fightMode && board->randomUnitFloat() <= expRate) {
        action = availableActions[board->randomInt(availableActionsCount)];
    } else {
        float maxValue = -9999.0f;
        for (int i = 0; i < availableActionsCount; i++) {
            std::string nextHash = board->getStateHash(tag, availableActions[i]);

            auto pair = svPairs.find(nextHash);
            float value = pair != svPairs.end() ? pair->second : 0.0f;

            if (debugMode)
                printf("[DEBUG] Evaluating action (%d, %d): %.4f\n", availableActions[i].x, availableActions[i].y,
                       value);

            if (value >= maxValue) {
                maxValue = value;
                action = availableActions[i];
            }
        }

        if (debugMode) printf("[DEBUG] Chosen action (%d, %d): %.4f\n", action.x, action.y, maxValue);
    }

    delete[] availableActions;
    return action;
}

void Agent::feedReward(float reward) {
    float _reward = reward;
    for (int i = gameStatesSize - 1; i >= 0; i--) {
        std::string state = gameStates[i];

        auto pair = svPairs.find(state);
        if (pair != svPairs.end()) {
            pair->second += learningRate * (decayGamma * _reward - pair->second);
            _reward = pair->second;
        } else {
            float initialValue = learningRate * (decayGamma * _reward);
            svPairs.insert(std::pair<std::string, float>(state, initialValue));
            _reward = 0.0f;
        }
    }
}

void Agent::addGameState(std::string state) {
    if (gameStatesSize < board->cellsCount) {
        gameStates[gameStatesSize] = std::move(state);
        gameStatesSize++;
    } else throw std::out_of_range("gameStates array filled.");
}

void Agent::newGame() {
    gameStatesSize = 0;
}

void Agent::debug(bool full) {
    printf("Agent %p [%c]\n", this, tag);
    printf("svPairsSize: %lu\n", svPairs.size());

    if (full) {
        int i = 0;
        for (std::pair<std::string, float> entry: svPairs) {
            printf("[%d] '%s' --> %.6f\n", i++, entry.first.c_str(), entry.second);
        }
    }
}

void Agent::save(const std::string &fileName) {
    FILE *f;
    f = fopen(fileName.c_str(), "w");

    fprintf(f, "%d\n%d\n%c\n%f\n%f\n%f\n", board->l, board->winStr, tag, expRate, decayGamma, learningRate);
    for (std::pair<std::string, float> entry: svPairs) {
        fprintf(f, "%s\n", entry.first.c_str());
        fprintf(f, "%f\n", entry.second);
    }

    fclose(f);
}

void Agent::load(const std::string &fileName) {
    FILE *f;
    f = fopen(fileName.c_str(), "r");
    if (f == nullptr) {
        std::cout << "The file does not exist" << std::endl;
        exit(200);
    }

    newGame();

    int i = 0;
    size_t len = 0;
    char *line = nullptr;
    std::string lastState;
    while (getline(&line, &len, f) != -1) {
        switch (i) {
            case 2:
                tag = line[0];
                break;
            case 3:
                expRate = atof(line);
            case 4:
                decayGamma = atof(line);
            case 5:
                learningRate = atof(line);
            default:
                if (i % 2 == 0 && len > 0) lastState = std::string(line).substr(0, board->cellsCount);
                else svPairs.insert(std::pair<std::string, float>(lastState, atof(line)));
                break;
        }
        i++;
    }

    fclose(f);
}

Agent::~Agent() = default;