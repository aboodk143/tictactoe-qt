#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QCryptographicHash>

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    initDatabase();
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tic_tac_toe.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
        exit(1);
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT UNIQUE, password TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS game_history (id INTEGER PRIMARY KEY, user_id INTEGER, result TEXT, date TEXT, FOREIGN KEY(user_id) REFERENCES users(id))");
}

void LoginWindow::on_loginButton_clicked() {
    QString username = ui->usernameEdit->text();
    QString password = QString(QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        MainWindow *mainWindow = new MainWindow(userId);
        mainWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password");
    }
}

void LoginWindow::on_registerButton_clicked() {
    QString username = ui->usernameEdit->text();
    QString password = QString(QCryptographicHash::hash(ui->passwordEdit->text().toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec()) {
        QMessageBox::information(this, "Registration Successful", "You can now log in with your credentials");
    } else {
        QMessageBox::warning(this, "Registration Failed", "Username already exists");
    }
}
