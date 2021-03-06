# TicTacToe AI

A basic reinforcement learning AI with flexible tictactoe board size.

## đĨ Brief example
<img src="static/example4x4.gif" width="190" height="100"  alt="4x4"/>
<img src="static/example3x3.gif" width="220" height="100"  alt="3x3"/>

## đ¨ Building
1) Clone the repository `git clone https://github.com/Belluxx/TicTacToeAI.git`
2) Go into the repository `cd TicTacToeAI`
3) Make a building directory `mkdir build`
4) Go into the build directory `cd build`
5) Run CMake `cmake -DCMAKE_BUILD_TYPE=Release ..`
6) Run make `make`

## đšī¸ Usage
First of all you need to choose an option:
> [1] Load AI file to play against user\
> [2] Load AI file to play against another AI\
> [3] Benchmark an AI file\
> [4] Train new AI

If it is the first time that you run this project then you can either [download a pre-trained AI file](https://github.com/Belluxx/TicTacToeAI/releases/download/v1.0/pretrained_ai_files.7z) and choose option 1/2 or train a new one with option 4.

The option 3 is useful to check how strong your AI file is. It makes your AI play 100k games against a very weak agent that plays randomly and reports the result.

To train a new AI you will be asked for some tweaks:
1) **Board size**: the size of the tictactoe square; 3 means 3x3 square and 4 means 4x4 square.
2) **Streak to win**: The number of consecutive symbols needed to win, in the classic 3x3 square it is 3
3) **Training iterations**: how many times the AI will play against itself. Note that **with a 4x4 board the iterations will take much more time than a 3x3 and more of them are needed to make it play well**! For a 3x3 board i suggest 10 million iterations (It will take some minutes to complete) and for a 4x4 i suggest 200 million iterations (It will take some hours to complete)
4) **File name**: the name of the AI file that will be generated (if you put "test" the generated AIs for X and O will be respectively ai1_test ai2_test)

Then wait for the training to finish
