#ifndef GAME_H
#define GAME_H

#include <vector>

enum Player { NONE, PLAYER_X, PLAYER_O };
enum GameResult { ONGOING, WIN, DRAW };

class Game {
public:
    Game();
    std::vector<std::vector<Player>> getBoard() const;
    Player getCurrentPlayer() const;
    bool makeMove(int row, int col, Player player);
    void undoMove(int row, int col);
    GameResult checkGameResult() const;
    void reset();

private:
    std::vector<std::vector<Player>> board;
    Player currentPlayer;
    bool checkWin(Player player) const;
    bool checkDraw() const;
};

#endif // GAME_H
