#include "game.h"

Game::Game() : board(3, std::vector<Player>(3, NONE)), currentPlayer(PLAYER_X) {}

std::vector<std::vector<Player>> Game::getBoard() const {
    return board;
}

Player Game::getCurrentPlayer() const {
    return currentPlayer;
}

bool Game::makeMove(int row, int col, Player player) {
    if (board[row][col] == NONE) {
        board[row][col] = player;
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        return true;
    }
    return false;
}

void Game::undoMove(int row, int col) {
    board[row][col] = NONE;
    currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
}

GameResult Game::checkGameResult() const {
    if (checkWin(PLAYER_X)) return WIN;
    if (checkWin(PLAYER_O)) return WIN;
    if (checkDraw()) return DRAW;
    return ONGOING;
}

void Game::reset() {
    board = std::vector<std::vector<Player>>(3, std::vector<Player>(3, NONE));
    currentPlayer = PLAYER_X;
}

bool Game::checkWin(Player player) const {
    // Check rows
    for (int row = 0; row < 3; ++row) {
        if (board[row][0] == player && board[row][1] == player && board[row][2] == player) {
            return true;
        }
    }
    // Check columns
    for (int col = 0; col < 3; ++col) {
        if (board[0][col] == player && board[1][col] == player && board[2][col] == player) {
            return true;
        }
    }
    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }
    return false;
}

bool Game::checkDraw() const {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == NONE) {
                return false;
            }
        }
    }
    return true;
}
