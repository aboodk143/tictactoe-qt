#include "mainwindow.h"
#include "loginwindow.h"
#include "ui_mainwindow.h"
#include "gamewindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QApplication>  // For qApp->quit()
#include <QProcess>

MainWindow::MainWindow(int userId, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), userId(userId)  {
    ui->setupUi(this);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
    connect(ui->logoutButton, &QPushButton::clicked, this, &MainWindow::on_logoutButton_clicked);
    if (!initializeDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to initialize the database");
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
bool MainWindow::initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("game_database.db");

    if (!db.open()) {
        return false;
    }

    QSqlQuery query;
    // Assuming you have a table creation query if it does not exist
    query.exec("CREATE TABLE IF NOT EXISTS game_history ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "user_id INTEGER, "
               "result TEXT, "
               "date TEXT)");

    return true;
}
void MainWindow::on_exitButton_clicked()
{
    qApp->quit();
}
void MainWindow::on_logoutButton_clicked()
{
    QString program = QCoreApplication::applicationFilePath();
    QStringList arguments = QCoreApplication::arguments();
    QString workingDirectory = QDir::currentPath();

    QProcess::startDetached(program, arguments, workingDirectory);
    qApp->quit();
}
void MainWindow::on_playButton_clicked() {
    GameWindow *gameWindow = new GameWindow(this); // Assuming MainWindow is the parent
    gameWindow->setAttribute(Qt::WA_DeleteOnClose); // Delete gameWindow on close
    gameWindow->show();
    centralWidget()->hide();

}

void MainWindow::on_historyButton_clicked()
{
    ui->historyList->clear();
    QSqlQuery query;
    query.prepare("SELECT result, date FROM game_history WHERE user_id = ?");
    query.addBindValue(userId);

    if (query.exec()) {
        while (query.next()) {
            QString result = query.value(0).toString();
            QString date = query.value(1).toString();
            QListWidgetItem *item = new QListWidgetItem(date + ": " + result);
            ui->historyList->addItem(item);
        }
    } else {
        QMessageBox::warning(this, "Error", "Failed to retrieve game history");
    }
}

