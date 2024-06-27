#include "ai.h"
#include <limits.h>

AI::AI(Player aiPlayer) : aiPlayer(aiPlayer) {}

std::pair<int, int> AI::getBestMove(Game& game, int difficulty) {
    int bestScore = (aiPlayer == PLAYER_X) ? INT_MIN : INT_MAX;
    std::pair<int, int> bestMove = {-1, -1};

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (game.getBoard()[row][col] == NONE) {
                game.makeMove(row, col, aiPlayer);
                int score = minimax(game, difficulty, false);
                game.undoMove(row, col); // Undo move

                if ((aiPlayer == PLAYER_X && score > bestScore) || (aiPlayer == PLAYER_O && score < bestScore)) {
                    bestScore = score;
                    bestMove = {row, col};
                }
            }
        }
    }
    return bestMove;
}

int AI::minimax(Game& game, int depth, bool isMaximizing) {
    GameResult result = game.checkGameResult();
    if (result == WIN) return isMaximizing ? -1 : 1;
    if (result == DRAW) return 0;
    if (depth == 0) return evaluate(game);

    int bestScore = isMaximizing ? INT_MIN : INT_MAX;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (game.getBoard()[row][col] == NONE) {
                game.makeMove(row, col, isMaximizing ? aiPlayer : (aiPlayer == PLAYER_X ? PLAYER_O : PLAYER_X));
                int score = minimax(game, depth - 1, !isMaximizing);
                game.undoMove(row, col); // Undo move

                bestScore = isMaximizing ? std::max(bestScore, score) : std::min(bestScore, score);
            }
        }
    }
    return bestScore;
}

int AI::evaluate(Game& game) {
    Player opponent = (aiPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    int score = 0;

    // Evaluate rows
    for (int row = 0; row < 3; ++row) {
        score += evaluateLine(game.getBoard()[row][0], game.getBoard()[row][1], game.getBoard()[row][2]);
    }

    // Evaluate columns
    for (int col = 0; col < 3; ++col) {
        score += evaluateLine(game.getBoard()[0][col], game.getBoard()[1][col], game.getBoard()[2][col]);
    }

    // Evaluate diagonals
    score += evaluateLine(game.getBoard()[0][0], game.getBoard()[1][1], game.getBoard()[2][2]);
    score += evaluateLine(game.getBoard()[0][2], game.getBoard()[1][1], game.getBoard()[2][0]);

    return (aiPlayer == PLAYER_X) ? score : -score;
}

int AI::evaluateLine(Player p1, Player p2, Player p3) {
    int score = 0;

    // First cell
    if (p1 == aiPlayer) {
        score = 1;
    } else if (p1 == opponent) {
        score = -1;
    }

    // Second cell
    if (p2 == aiPlayer) {
        if (score == 1) {
            score = 10;
        } else if (score == -1) {
            return 0;
        } else {
            score = 1;
        }
    } else if (p2 == opponent) {
        if (score == -1) {
            score = -10;
        } else if (score == 1) {
            return 0;
        } else {
            score = -1;
        }
    }

    // Third cell
    if (p3 == aiPlayer) {
        if (score > 0) {
            score *= 10;
        } else if (score < 0) {
            return 0;
        } else {
            score = 1;
        }
    } else if (p3 == opponent) {
        if (score < 0) {
            score *= 10;
        } else if (score > 0) {
            return 0;
        } else {
            score = -1;
        }
    }

    return score;
}
