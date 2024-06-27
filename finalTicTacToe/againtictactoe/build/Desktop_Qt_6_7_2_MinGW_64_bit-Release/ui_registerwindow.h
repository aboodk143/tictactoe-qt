/********************************************************************************
** Form generated from reading UI file 'registerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWINDOW_H
#define UI_REGISTERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterWindow
{
public:
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *registerButton;

    void setupUi(QWidget *RegisterWindow)
    {
        if (RegisterWindow->objectName().isEmpty())
            RegisterWindow->setObjectName("RegisterWindow");
        RegisterWindow->resize(400, 300);
        usernameLabel = new QLabel(RegisterWindow);
        usernameLabel->setObjectName("usernameLabel");
        usernameLabel->setGeometry(QRect(40, 50, 71, 21));
        passwordLabel = new QLabel(RegisterWindow);
        passwordLabel->setObjectName("passwordLabel");
        passwordLabel->setGeometry(QRect(40, 100, 71, 21));
        usernameEdit = new QLineEdit(RegisterWindow);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(120, 50, 200, 21));
        passwordEdit = new QLineEdit(RegisterWindow);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setGeometry(QRect(120, 100, 200, 21));
        passwordEdit->setEchoMode(QLineEdit::Password);
        registerButton = new QPushButton(RegisterWindow);
        registerButton->setObjectName("registerButton");
        registerButton->setGeometry(QRect(160, 150, 75, 23));

        retranslateUi(RegisterWindow);

        QMetaObject::connectSlotsByName(RegisterWindow);
    } // setupUi

    void retranslateUi(QWidget *RegisterWindow)
    {
        RegisterWindow->setWindowTitle(QCoreApplication::translate("RegisterWindow", "Register", nullptr));
        usernameLabel->setText(QCoreApplication::translate("RegisterWindow", "Username:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("RegisterWindow", "Password:", nullptr));
        registerButton->setText(QCoreApplication::translate("RegisterWindow", "Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterWindow: public Ui_RegisterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOW_H
