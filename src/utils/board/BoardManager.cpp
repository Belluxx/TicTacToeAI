#include <iostream>
#include <thread>
#include "BoardManager.h"

BoardManager::BoardManager() = default;

/**
 * Train two agents
 * @param l The size of the board
 * @param winStr The number of consecutive symbols needed to win
 * @param iterations The number of games that the agents will play
 */
void BoardManager::train(int l, int winStr, int iterations) {
    makeBoard(l, winStr);

    Agent ai1 = Agent(&board, Board::X);
    Agent ai2 = Agent(&board, Board::O);

    if (ai1.tag == ai2.tag) {
        std::cout << "Incompatible AIs: same tags" << std::endl;
        exit(300);
    }

    std::string fileName;
    std::cout << "File name: ";
    fflush(stdin);
    std::getline(std::cin, fileName);

    pos action;
    int status;
    for (int game = 0; game < iterations; game++) {
        if (game % 100000 == 0) printf("Iteration: %dk\n", game / 1000);
        board.reset();

        do {
            if (board.turn == ai1.tag) {
                action = ai1.chooseAction();
                board.performAction(board.turn, action);
                ai1.addGameState(board.getStateHash());
            } else {
                action = ai2.chooseAction();
                board.performAction(board.turn, action);
                ai2.addGameState(board.getStateHash());
            }

            status = board.getGameStatus();
        } while (status == 0);

        if (status == 1) {
            ai1.feedReward(1.0);
            ai2.feedReward(-0.5);
        } else if (status == 2) {
            ai1.feedReward(-0.5);
            ai2.feedReward(1.0);
        } else if (status == 3) {
            ai1.feedReward(0.3);
            ai2.feedReward(0.3);
        } else printf("An error occurred\n");

        ai1.newGame();
        ai2.newGame();
    }

    std::cout << "Training finished.\n";
    std::cout << "\nAI1 Agent info:\n";
    ai1.debug();
    std::cout << "\nAI2 Agent info:\n";
    ai2.debug();

    std::cout << "\nSaving AI1 Agent...\n";
    ai1.save(std::string("ai1_").append(fileName));
    std::cout << "Saving AI2 Agent...\n";
    ai2.save(std::string("ai2_").append(fileName));
}

/**
 * Play an AI vs Human game
 * @param aiFile The name of the ai file
 * @param debugMode If true, more info will be shown
 */
void BoardManager::AiVsHuman(const std::string &aiFile, bool debugMode) {
    makeBoard(aiFile);
    Agent ai = Agent(&board);
    ai.load(aiFile);

    int status;
    char opt;
    pos action;
    do {
        board.reset();
        ai.newGame();

        do {
            printf("\n");
            board.print();

            do {
                if (board.turn == ai.tag) {
                    action = ai.chooseAction(true, debugMode);
                } else {
                    printf("\n[%c] Choose coords (x y): ", board.turn);
                    std::cin >> action.x;
                    std::cin >> action.y;
                    fflush(stdin);
                }
            } while (!board.performAction(board.turn, action));

            status = board.getGameStatus();
        } while (status == 0);

        printf("\n");
        board.print();
        if (status == 1) printf("\n>>> X Won! <<<\n");
        else if (status == 2) printf("\n>>> O Won! <<<\n");
        else if (status == 3) printf("\n>>> Draw! <<<\n");
        else printf("\nAn error occurred\n");

        std::cout << "Play again? (y/n): ";
        std::cin >> opt;
    } while (opt == 'y');
}

/**
 * Play an AI vs AI game
 * @param ai1File The name of the first ai file
 * @param ai2File The name of the second ai file
 */
void BoardManager::AiVsAi(const std::string &ai1File, const std::string &ai2File) {
    makeBoard(ai1File);
    makeBoard(ai2File);

    Agent ai1 = Agent(&board);
    Agent ai2 = Agent(&board);
    ai1.load(ai1File);
    ai2.load(ai2File);

    if (ai1.tag == ai2.tag) {
        std::cout << "Incompatible AIs: same tags" << std::endl;
        exit(300);
    }

    int status;
    char opt;
    pos action;
    do {
        board.reset();
        ai1.newGame();
        ai2.newGame();

        do {
            delay(3);
            printf("\n");
            board.print();

            if (board.turn == ai1.tag) action = ai1.chooseAction(true);
            else action = ai2.chooseAction(true);
            board.performAction(board.turn, action);

            status = board.getGameStatus();
        } while (status == 0);

        printf("\n");
        board.print();
        if (status == 1) printf("\n>>> X Won! <<<\n");
        else if (status == 2) printf("\n>>> O Won! <<<\n");
        else if (status == 3) printf("\n>>> Draw! <<<\n");
        else printf("\nAn error occurred\n");

        std::cout << "Watch again? (y/n): ";
        std::cin >> opt;
    } while (opt == 'y');
}

/**
 * Create a board
 * @param l Size of the board
 * @param winStr Number of consecutive symbols needed to win
 */
void BoardManager::makeBoard(int l, int winStr) {
    this->board = Board(l, winStr);

    this->board.reset();
}

/**
 * Create a board
 * @param fileName name of the agent file
 */
void BoardManager::makeBoard(const std::string &fileName) {
    int currL = board.l;
    int currWinStr = board.winStr;
    int newL = 0;
    int newWinStr = 0;

    FILE *f;
    f = fopen(fileName.c_str(), "r");
    if (f == nullptr) {
        std::cout << "The file does not exist" << std::endl;
        exit(200);
    }
    int i = 0;
    size_t len = 0;
    char *line = nullptr;
    while (getline(&line, &len, f) != -1) {
        if (i == 0) newL = atoi(line);
        else if (i == 1) newWinStr = atoi(line);
        i++;
    }
    fclose(f);

    if (currL == 0 && currWinStr == 0) {
        this->board = Board(newL, newWinStr);
    } else if (currL != newL || currWinStr != newWinStr) {
        std::cout << "Incompatible AIs: different board parameters" << std::endl;
        exit(300);
    }
}

/**
 * Pauses the execution
 * @param seconds Number of seconds to wait
 */
void BoardManager::delay(int seconds) {
    std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(seconds));
}