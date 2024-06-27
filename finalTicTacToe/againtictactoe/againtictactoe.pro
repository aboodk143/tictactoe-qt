QT += core gui widgets sql
CONFIG += c++11

# Define your source and header files
SOURCES += \
    database.cpp \
    gamewindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    registerwindow.cpp

HEADERS += \
    database.h \
    gamewindow.h \
    loginwindow.h \
    mainwindow.h \
    registerwindow.h

FORMS += \
    gamewindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    registerwindow.ui

# Add the path to your SQLite headers and library
INCLUDEPATH += C:/sqlite
LIBS += -L C:/sqlite -lsqlite3
