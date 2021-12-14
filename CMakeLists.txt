cmake_minimum_required(VERSION 3.20)
project(TicTacToeAI)

set(CMAKE_CXX_STANDARD 11)

add_executable(TicTacToeAI src/main.cpp src/utils/board/Board.cpp src/utils/board/Board.h src/utils/ai/Agent.cpp src/utils/ai/Agent.h src/utils/board/BoardManager.cpp src/utils/board/BoardManager.h)
