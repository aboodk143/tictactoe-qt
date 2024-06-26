#ifndef AI_H
#define AI_H

#include "game.h"
#include <utility>

class AI {
public:
    AI(Player aiPlayer);
    std::pair<int, int> getBestMove(Game& game, int difficulty);

private:
    Player aiPlayer;
    Player opponent;
    int minimax(Game& game, int depth, bool isMaximizing);
    int evaluate(Game& game);
    int evaluateLine(Player p1, Player p2, Player p3);
};

#endif // AI_H
