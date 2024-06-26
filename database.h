#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>

class Database {
public:
    Database();
    bool registerUser(const QString& username, const QString& password);
    bool loginUser(const QString& username, const QString& password);

private:
    QSqlDatabase db;
    QString hashPassword(const QString& password) const;
};

#endif // DATABASE_H
