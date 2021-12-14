#include <iostream>
#include "utils/ai/Agent.h"
#include "utils/board/BoardManager.h"

int main() {
    int opt = 0;
    std::cout << "[1] Load AI file to play against user\n";
    std::cout << "[2] Load AI file to play against another AI\n";
    std::cout << "[3] Train new AI\n";
    std::cout << "Choose an option: ";
    std::cin >> opt;

    BoardManager bm = BoardManager();

    switch (opt) {
        case 1: {
            char debugMode;
            std::string fileName;

            std::cout << "File name: ";
            fflush(stdin);
            std::getline(std::cin, fileName);
            std::cout << "Show debug info? (y/n): ";
            std::cin >> debugMode;

            bm.AiVsHuman(fileName, debugMode == 'y');
            break;
        }

        case 2: {
            std::string ai1FileName, ai2FileName;

            std::cout << "AI1 file name: ";
            fflush(stdin);
            std::getline(std::cin, ai1FileName);
            std::cout << "AI2 file name: ";
            fflush(stdin);
            std::getline(std::cin, ai2FileName);

            BoardManager bm = BoardManager();
            bm.AiVsAi(ai1FileName, ai2FileName);
            break;
        }

        case 3: {
            int boardSize, winStr, trainIterations;

            std::cout << "Board size: ";
            std::cin >> boardSize;
            std::cout << "Streak to win: ";
            std::cin >> winStr;
            std::cout << "Training iterations: ";
            std::cin >> trainIterations;

            bm.train(boardSize, winStr, trainIterations);
            break;
        }

        default: {
            std::cout << "Option not valid.\n";
            exit(1);
        }
    }
}