#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamewindow.h"
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(int userId, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_playButton_clicked();
    void on_historyButton_clicked();
    void on_exitButton_clicked();    // Slot for exit button
    void on_logoutButton_clicked();  // Slot for logout button

private:
    Ui::MainWindow *ui;
    int userId;
    bool initializeDatabase();  // Function to initialize the database
};

#endif // MAINWINDOW_H
