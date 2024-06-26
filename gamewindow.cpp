#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDebug>
#include <QSqlQuery> // Add this include statement
#include <QSqlError>
GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
    , currentPlayer("X")
    , againstAI(false)
    , aiDifficultyHard(false)
{
    ui->setupUi(this);

    board = ui->board;
    modeCheckBox = ui->modeCheckBox;
    difficultyCheckBox = ui->difficultyCheckBox;

    connect(board, &QTableWidget::cellClicked, this, &GameWindow::on_board_cellClicked);
    connect(modeCheckBox, &QCheckBox::stateChanged, this, &GameWindow::on_modeCheckBox_stateChanged);
    connect(difficultyCheckBox, &QCheckBox::stateChanged, this, &GameWindow::on_difficultyCheckBox_stateChanged);
    connect(ui->restartButton, &QPushButton::clicked, this, &GameWindow::on_restartButton_clicked);  // Connect the restart button

    // Create and connect the history button
    historyButton = new QPushButton("History", this);
    connect(historyButton, &QPushButton::clicked, this, &GameWindow::on_historyButton_clicked);

    board->setRowCount(3);
    board->setColumnCount(3);
    for (int i = 0; i < 3; ++i) {
        board->setColumnWidth(i, 100);
        board->setRowHeight(i, 100);
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

void GameWindow::on_board_cellClicked(int row, int column)
{
    if (!board->item(row, column)) {
        if (!againstAI || (currentPlayer == "X")) {
            board->setItem(row, column, new QTableWidgetItem(currentPlayer));
            checkGameOver();
            if (againstAI && currentPlayer == "X") {
                aiMove();
            }
            switchPlayer();
        }
    }
}

void GameWindow::on_modeCheckBox_stateChanged(int state)
{
    againstAI = (state == Qt::Checked);
    resetBoard();
}

void GameWindow::on_difficultyCheckBox_stateChanged(int state)
{
    aiDifficultyHard = (state == Qt::Checked);
}

void GameWindow::switchPlayer()
{
    currentPlayer = (currentPlayer == "X") ? "O" : "X";
}

void GameWindow::resetBoard()
{
    board->clear();
    currentPlayer = "X";
}
void GameWindow::on_historyButton_clicked()
{
    QSqlQuery query;
    query.exec("SELECT player, wins, losses, draws FROM statistics");

    QString history = "Player Statistics:\n";
    while (query.next()) {
        QString player = query.value(0).toString();
        int wins = query.value(1).toInt();
        int losses = query.value(2).toInt();
        int draws = query.value(3).toInt();

        history += QString("%1 - Wins: %2, Losses: %3, Draws: %4\n").arg(player).arg(wins).arg(losses).arg(draws);
    }

    QMessageBox::information(this, "History", history);
}
void GameWindow::gameOver(const QString &winner)
{
    QString message;
    if (winner == "Draw") {
        message = "It's a draw!";
        // Update draw counts
        QSqlQuery query;
        query.prepare("UPDATE statistics SET draws = draws + 1 WHERE player = 'X'");
        query.exec();
        query.prepare("UPDATE statistics SET draws = draws + 1 WHERE player = 'O'");
        query.exec();
    } else {
        message = winner + " wins!";
        // Update win/loss counts
        QSqlQuery query;
        query.prepare("UPDATE statistics SET wins = wins + 1 WHERE player = ?");
        query.addBindValue(winner);
        query.exec();

        QString loser = (winner == "X") ? "O" : "X";
        query.prepare("UPDATE statistics SET losses = losses + 1 WHERE player = ?");
        query.addBindValue(loser);
        query.exec();
    }

    QMessageBox::information(this, "Game Over", message);

    // Clear the board completely
    for (int i = 0; i < board->rowCount(); ++i) {
        for (int j = 0; j < board->columnCount(); ++j) {
            QTableWidgetItem* item = board->takeItem(i, j);
            delete item;  // Properly delete the item
        }
    }

    // Reset current player to X
    currentPlayer = "X";

    // Reset checkboxes
    ui->modeCheckBox->setChecked(false);
    ui->difficultyCheckBox->setChecked(false);

    // Reset game mode and difficulty
    againstAI = false;
    aiDifficultyHard = false;

    // Save the result to the database
    QSqlQuery query;
    query.prepare("INSERT INTO games (winner) VALUES (?)");
    query.addBindValue(winner);
    query.exec();
}




void GameWindow::aiMove()
{
    if (!isBoardFull()) {
        if (aiDifficultyHard) {
            aiMoveHard();
        } else {
            aiMoveEasy();
        }
        checkGameOver();
        switchPlayer();
    }
}

void GameWindow::aiMoveEasy()
{
    // Implement easy AI move (random move)
    int row, col;
    do {
        row = std::rand() % 3;
        col = std::rand() % 3;
    } while (board->item(row, col));

    board->setItem(row, col, new QTableWidgetItem("O"));
}

void GameWindow::aiMoveHard()
{
    // Implement hard AI move (minimax algorithm)
    int bestScore = INT_MIN;
    int bestRow = -1;
    int bestCol = -1;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!board->item(i, j)) {
                board->setItem(i, j, new QTableWidgetItem("O"));
                int score = minimax(false);
                board->setItem(i, j, nullptr);
                if (score > bestScore) {
                    bestScore = score;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        board->setItem(bestRow, bestCol, new QTableWidgetItem("O"));
    }
}

int GameWindow::minimax(bool isMaximizing)
{
    QString result = checkWin();
    if (result == "X") {
        return -10;
    } else if (result == "O") {
        return 10;
    } else if (isBoardFull()) {
        return 0;
    }

    int bestScore = isMaximizing ? INT_MIN : INT_MAX;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!board->item(i, j)) {
                board->setItem(i, j, new QTableWidgetItem(isMaximizing ? "O" : "X"));
                int score = minimax(!isMaximizing);
                board->setItem(i, j, nullptr);
                bestScore = isMaximizing ? qMax(bestScore, score) : qMin(bestScore, score);
            }
        }
    }

    return bestScore;
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
            if (!board->item(i, j)) {
                return false;
            }
        }
    }
    return true;
}

QString GameWindow::checkWin()
{
    for (int i = 0; i < 3; ++i) {
        if (board->item(i, 0) && board->item(i, 1) && board->item(i, 2) &&
            board->item(i, 0)->text() == board->item(i, 1)->text() &&
            board->item(i, 1)->text() == board->item(i, 2)->text()) {
            return board->item(i, 0)->text();
        }
        if (board->item(0, i) && board->item(1, i) && board->item(2, i) &&
            board->item(0, i)->text() == board->item(1, i)->text() &&
            board->item(1, i)->text() == board->item(2, i)->text()) {
            return board->item(0, i)->text();
        }
    }
    if (board->item(0, 0) && board->item(1, 1) && board->item(2, 2) &&
        board->item(0, 0)->text() == board->item(1, 1)->text() &&
        board->item(1, 1)->text() == board->item(2, 2)->text()) {
        return board->item(0, 0)->text();
    }
    if (board->item(0, 2) && board->item(1, 1) && board->item(2, 0) &&
        board->item(0, 2)->text() == board->item(1, 1)->text() &&
        board->item(1, 1)->text() == board->item(2, 0)->text()) {
        return board->item(0, 2)->text();
    }
    return "";
}
void GameWindow::on_restartButton_clicked()
{
    resetGame();
}
void GameWindow::resetGame()
{
    // Clear the board completely
    for (int i = 0; i < board->rowCount(); ++i) {
        for (int j = 0; j < board->columnCount(); ++j) {
            QTableWidgetItem* item = board->takeItem(i, j);
            delete item;  // Properly delete the item
        }
    }

    // Reset current player to X
    currentPlayer = "X";

    // Reset checkboxes
    ui->modeCheckBox->setChecked(false);
    ui->difficultyCheckBox->setChecked(false);

    // Reset game mode and difficulty
    againstAI = false;
    aiDifficultyHard = false;
}

