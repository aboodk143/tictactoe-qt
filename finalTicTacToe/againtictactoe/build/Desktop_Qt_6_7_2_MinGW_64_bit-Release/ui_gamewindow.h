/********************************************************************************
** Form generated from reading UI file 'gamewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOW_H
#define UI_GAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWindow
{
public:
    QCheckBox *modeCheckBox;
    QCheckBox *difficultyCheckBox;
    QPushButton *restartButton;
    QPushButton *historyButton;

    void setupUi(QWidget *GameWindow)
    {
        if (GameWindow->objectName().isEmpty())
            GameWindow->setObjectName("GameWindow");
        GameWindow->resize(678, 416);
        modeCheckBox = new QCheckBox(GameWindow);
        modeCheckBox->setObjectName("modeCheckBox");
        modeCheckBox->setGeometry(QRect(140, 20, 78, 22));
        difficultyCheckBox = new QCheckBox(GameWindow);
        difficultyCheckBox->setObjectName("difficultyCheckBox");
        difficultyCheckBox->setGeometry(QRect(240, 20, 78, 22));
        restartButton = new QPushButton(GameWindow);
        restartButton->setObjectName("restartButton");
        restartButton->setGeometry(QRect(350, 50, 80, 24));
        historyButton = new QPushButton(GameWindow);
        historyButton->setObjectName("historyButton");
        historyButton->setGeometry(QRect(350, 10, 80, 24));

        retranslateUi(GameWindow);

        QMetaObject::connectSlotsByName(GameWindow);
    } // setupUi

    void retranslateUi(QWidget *GameWindow)
    {
        GameWindow->setWindowTitle(QCoreApplication::translate("GameWindow", "Game", nullptr));
        modeCheckBox->setText(QCoreApplication::translate("GameWindow", "AI", nullptr));
        difficultyCheckBox->setText(QCoreApplication::translate("GameWindow", "hARD", nullptr));
        restartButton->setText(QCoreApplication::translate("GameWindow", "Restart", nullptr));
        historyButton->setText(QCoreApplication::translate("GameWindow", "History", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameWindow: public Ui_GameWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOW_H
