#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QSqlDatabase>
#include <QSqlError>

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void on_buttonClicked();
    void on_modeCheckBox_stateChanged(int state);
    void on_difficultyCheckBox_stateChanged(int state);
    void on_restartButton_clicked();
    void on_historyButton_clicked(); // Declare the new slot

private:
    Ui::GameWindow *ui;
    QPushButton* board[3][3];
    QCheckBox *modeCheckBox;
    QCheckBox *difficultyCheckBox;
    QPushButton *historyButton;
    QString currentPlayer;
    bool againstAI;
    bool aiDifficultyHard;
    QSqlDatabase db;
    void resetBoard();
    void switchPlayer();
    void gameOver(const QString &winner);
    void aiMove();
    void aiMoveEasy();
    void aiMoveHard();
    int minimax(bool isMaximizing);
    void checkGameOver();
    bool isBoardFull();
    QString checkWin();
    void resetGame();
};

#endif // GAMEWINDOW_H
