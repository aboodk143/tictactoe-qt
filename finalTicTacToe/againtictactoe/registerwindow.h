#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void on_registerButton_clicked();

private:
    Ui::RegisterWindow *ui;
    QSqlDatabase db;
    bool createUser(const QString &username, const QString &password);
};

#endif // REGISTERWINDOW_H
