#include "registerwindow.h"
#include "ui_registerwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QSqlError> // Include QSqlError header

RegisterWindow::RegisterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("tictactoe.db");
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_registerButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Username and password cannot be empty");
        return;
    }

    if (createUser(username, password)) {
        QMessageBox::information(this, "Registration Successful", "User registered successfully");
        this->close();
    } else {
        QMessageBox::critical(this, "Registration Error", "Failed to register user");
    }
}

bool RegisterWindow::createUser(const QString &username, const QString &password)
{
    QString hashedPassword = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    return query.exec();
}
