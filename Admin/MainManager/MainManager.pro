#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T20:05:39
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainManager
TEMPLATE = app

INCLUDEPATH += ../../common/connectionWidget/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp

HEADERS  += mainwindow.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h

FORMS    += mainwindow.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui

RESOURCES += \
    ../../common/connectionWidget/res.qrc
