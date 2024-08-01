#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

char board[3][3];
char _player = 'X', _computer = 'O';

void printBoard() {
    std::cout << std::endl;
    std::cout << ' ' << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << ' ' << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << std::endl;
    std::cout << "-----------" << std::endl;
    std::cout << ' ' << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << std::endl;
    std::cout << std::endl;
}

std::vector<std::pair<size_t, size_t>> getMoves() {
    std::vector<std::pair<size_t, size_t>> moves;
    
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            if (board[i][j] != _computer && board[i][j] != _player) {
                moves.push_back(std::make_pair(i, j));
            }
        }
    }

    return moves;
}

bool gameWon(char marker) {
    return (board[0][0] == marker && board[0][1] == marker && board[0][2] == marker ||
            board[1][0] == marker && board[1][1] == marker && board[1][2] == marker ||
            board[2][0] == marker && board[2][1] == marker && board[2][2] == marker) ||
           (board[0][0] == marker && board[1][0] == marker && board[2][0] == marker ||
            board[0][1] == marker && board[1][1] == marker && board[2][1] == marker ||
            board[0][2] == marker && board[1][2] == marker && board[2][2] == marker) ||
           (board[0][0] == marker && board[1][1] == marker && board[2][2] == marker ||
            board[0][2] == marker && board[1][1] == marker && board[2][0] == marker);
}

bool positionOccupied(size_t row, size_t col) {
    return board[row][col] == _computer || board[row][col] == _player;
}

bool boardFull() {
    std::vector<std::pair<size_t, size_t>> moves = getMoves();
    return moves.size() == 0;
}

char getOpponentMarker(char marker) {
    return (marker == _player) ? _computer : _player;
}

int getBoardState(char marker, int depth) {
    if (gameWon(marker)) {
        return 10 - depth;
    }

    if (gameWon(getOpponentMarker(marker))) {
        return depth - 10;
    }

    if (boardFull()) {
        return 0;
    }

    return 0;
}

bool gameOver(int depth) {
    return boardFull() || getBoardState(_computer, depth) != 0;
}

std::pair<int, std::pair<int, int>> minimax(char marker, int depth, int alpha, int beta) {
    std::pair<int, int> bestMove = std::make_pair(-1, -1);
    int bestScore = (marker == _computer) ? -100 : 100;
    
    if (gameOver(depth)) {
        bestScore = getBoardState(_computer, depth);
        return std::make_pair(bestScore, bestMove);
    }

    std::vector<std::pair<size_t, size_t>> moves = getMoves();

    for (size_t i = 0; i < moves.size(); ++i) {
        std::pair<int, int> crrMove = moves[i];

        board[crrMove.first][crrMove.second] = marker;

        if (marker == _computer) {
            // maximize player's turn
            int score = minimax(_player, depth + 1, alpha, beta).first;

            if (bestScore < score) {
                bestScore = score;
                bestMove = crrMove;

                alpha = std::max(alpha, bestScore);
                board[crrMove.first][crrMove.second] = ' ';

                if (alpha >= beta) {
                    break;
                }
            }

        }
        else if(marker == _player) { 
            // minimize opponent's turn
            int score = minimax(_computer, depth + 1, alpha, beta).first;

            if (bestScore > score) {
                bestScore = score;
                bestMove = crrMove;

                beta = std::min(beta, bestScore);
                board[crrMove.first][crrMove.second] = ' ';

                if (alpha >= beta) {
                    break;
                }
            }
        }

        board[crrMove.first][crrMove.second] = ' ';
    }

    return std::make_pair(bestScore, bestMove);
}

int main() {
    bool flag = false;
    std::cout << "Do you want to play first?" << std::endl;
    std::cin >> flag;
    if (!std::cin) { return -1; }

    for (size_t i = 0; i < 3; ++i) { 
        for(size_t j = 0; j < 3; ++j) { 
            board[i][j] = ' '; 
        } 
    }

    if (!flag) {
        _computer = 'X';
        _player = 'O';

        std::srand(std::time(0));
        size_t random = std::rand() % 5;

        switch (random) {
            case 0: board[0][0] = _computer; break;
            case 1: board[0][2] = _computer; break;
            case 2: board[2][0] = _computer; break; 
            case 3: board[2][2] = _computer; break;
            case 4: board[1][1] = _computer; break;
        }
    }

    printBoard();

    int row, col;

    while (!gameOver(0)) {
        std::cout << "Please enter row:" << std::endl;
        std::cin >> row;
        std::cout << "Please enter col:" << std::endl;
        std::cin >> col;

        if (!std::cin) {
            std::cout << "Incorrect input, exiting..." << std::endl;
            return -1;    
        }
        else if ((row <= 0 || row > 3) || (col <= 0 || col > 3)) {
            std::cout << "Out of board, try again..." << std::endl;
            continue;
        }
        else if (positionOccupied(row - 1, col - 1)) {
            std::cout << "Position occupied, try again..." << std::endl;
            continue;
        }
        else {
            board[row - 1][col - 1] = _player;
        }

        std::pair<int, std::pair<int, int>> move = minimax(_computer, 1, -100, 100);
        board[move.second.first][move.second.second] = _computer;
        
        printBoard();
    }

    int result = getBoardState(_player, 0);
    
    if (result == 1) {
        std::cout << "You won! :)" << std::endl;
    }
    else if (result == -1) {
        std::cout << "You lost! :(" << std::endl;
    }
    else {
        std::cout << "Draw!" << std::endl;
    }

    return 0;
}