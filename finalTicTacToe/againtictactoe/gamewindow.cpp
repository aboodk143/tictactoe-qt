#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
    , currentPlayer("X")
    , againstAI(false)
    , aiDifficultyHard(false)
{
    ui->setupUi(this);

    modeCheckBox = ui->modeCheckBox;
    difficultyCheckBox = ui->difficultyCheckBox;

    connect(modeCheckBox, &QCheckBox::stateChanged, this, &GameWindow::on_modeCheckBox_stateChanged);
    connect(difficultyCheckBox, &QCheckBox::stateChanged, this, &GameWindow::on_difficultyCheckBox_stateChanged);
    connect(ui->restartButton, &QPushButton::clicked, this, &GameWindow::on_restartButton_clicked);

    historyButton = new QPushButton("History", this);
    historyButton->move(400, 50); // Adjust the position as needed
    connect(historyButton, &QPushButton::clicked, this, &GameWindow::on_historyButton_clicked);

    int startX = 50;
    int startY = 100; // Adjusted Y position to leave space for other UI elements
    int buttonSize = 100;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = new QPushButton(this);
            board[i][j]->setFixedSize(buttonSize, buttonSize);
            board[i][j]->move(startX + buttonSize * j, startY + buttonSize * i);
            connect(board[i][j], &QPushButton::clicked, this, &GameWindow::on_buttonClicked);
        }
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tictactoe.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS games (id INTEGER PRIMARY KEY, winner TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS statistics (player TEXT PRIMARY KEY, wins INTEGER, losses INTEGER, draws INTEGER)");
    query.exec("INSERT OR IGNORE INTO statistics (player, wins, losses, draws) VALUES ('X', 0, 0, 0)");
    query.exec("INSERT OR IGNORE INTO statistics (player, wins, losses, draws) VALUES ('O', 0, 0, 0)");
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::on_buttonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button || !button->text().isEmpty())
        return;

    button->setText(currentPlayer);
    checkGameOver();
    switchPlayer();
}

void GameWindow::resetGame()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j]->setText("");
        }
    }

    currentPlayer = "X";
    ui->modeCheckBox->setChecked(false);
    ui->difficultyCheckBox->setChecked(false);
    againstAI = false;
    aiDifficultyHard = false;
}

void GameWindow::switchPlayer()
{
    currentPlayer = (currentPlayer == "X") ? "O" : "X";
    if (againstAI && currentPlayer == "O") {
        aiMove();
    }
}

void GameWindow::checkGameOver()
{
    QString winner = checkWin();
    if (!winner.isEmpty()) {
        gameOver(winner);

    } else if (isBoardFull()) {
        gameOver("Draw");

    }
}

bool GameWindow::isBoardFull()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j]->text().isEmpty())
                return false;
        }
    }
    return true;
}

QString GameWindow::checkWin()
{
    for (int i = 0; i < 3; ++i) {
        if (!board[i][0]->text().isEmpty() &&
            board[i][0]->text() == board[i][1]->text() &&
            board[i][1]->text() == board[i][2]->text())
            return board[i][0]->text();
        if (!board[0][i]->text().isEmpty() &&
            board[0][i]->text() == board[1][i]->text() &&
            board[1][i]->text() == board[2][i]->text())
            return board[0][i]->text();
    }

    if (!board[0][0]->text().isEmpty() &&
        board[0][0]->text() == board[1][1]->text() &&
        board[1][1]->text() == board[2][2]->text())
        return board[0][0]->text();
    if (!board[0][2]->text().isEmpty() &&
        board[0][2]->text() == board[1][1]->text() &&
        board[1][1]->text() == board[2][0]->text())
        return board[0][2]->text();

    return "";
}

void GameWindow::gameOver(const QString &winner)
{
    QString message = (winner == "Draw") ? "It's a draw!" : winner + " wins!";
    QMessageBox::information(this, "Game Over", message);

    QSqlQuery query;
    if (winner != "Draw") {
        query.prepare("INSERT INTO games (winner) VALUES (:winner)");
        query.bindValue(":winner", winner);
        query.exec();

        query.prepare("UPDATE statistics SET wins = wins + 1 WHERE player = :player");
        query.bindValue(":player", winner);
        query.exec();

        QString loser = (winner == "X") ? "O" : "X";
        query.prepare("UPDATE statistics SET losses = losses + 1 WHERE player = :player");
        query.bindValue(":player", loser);
        query.exec();
    } else {
        query.prepare("UPDATE statistics SET draws = draws + 1 WHERE player IN ('X', 'O')");
        query.exec();
    }

    resetGame();
}

void GameWindow::on_modeCheckBox_stateChanged(int state)
{
    againstAI = (state == Qt::Checked);
}

void GameWindow::on_difficultyCheckBox_stateChanged(int state)
{
    aiDifficultyHard = (state == Qt::Checked);
}

void GameWindow::on_restartButton_clicked()
{
    resetGame();
}

void GameWindow::on_historyButton_clicked()
{
    // Handle history button click event
    QSqlQuery query;
    query.exec("SELECT * FROM games");

    QString history;
    while (query.next()) {
        QString winner = query.value(1).toString();
        history += "Game " + query.value(0).toString() + ": " + winner + " wins\n";
    }

    QMessageBox::information(this, "Game History", history);
}

void GameWindow::aiMove()
{
    if (aiDifficultyHard) {
        aiMoveHard();
    } else {
        aiMoveEasy();
    }
}

void GameWindow::aiMoveEasy()
{
    QVector<QPushButton*> emptyButtons;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j]->text().isEmpty()) {
                emptyButtons.append(board[i][j]);
            }
        }
    }

    if (!emptyButtons.isEmpty()) {
        int index = QRandomGenerator::global()->bounded(emptyButtons.size());
        emptyButtons.at(index)->setText(currentPlayer);
        checkGameOver();
        switchPlayer();
    }
}

void GameWindow::aiMoveHard()
{
    int bestScore = -1000;
    QPushButton* bestMove = nullptr;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j]->text().isEmpty()) {
                board[i][j]->setText(currentPlayer);
                int score = minimax(false);
                board[i][j]->setText("");
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = board[i][j];
                }
            }
        }
    }

    if (bestMove) {
        bestMove->setText(currentPlayer);
        checkGameOver();
        switchPlayer();
    }
}

int GameWindow::minimax(bool isMaximizing)
{
    QString result = checkWin();
    if (result == "X") return -1;
    if (result == "O") return 1;
    if (isBoardFull()) return 0;

    int bestScore = isMaximizing ? -1000 : 1000;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j]->text().isEmpty()) {
                board[i][j]->setText(isMaximizing ? "O" : "X");
                int score = minimax(!isMaximizing);
                board[i][j]->setText("");
                if (isMaximizing) {
                    bestScore = std::max(score, bestScore);
                } else {
                    bestScore = std::min(score, bestScore);
                }
            }
        }
    }

    return bestScore;
}
