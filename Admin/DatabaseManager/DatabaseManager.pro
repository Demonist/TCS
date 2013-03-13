#-------------------------------------------------
#
# Project created by QtCreator 2013-03-12T23:11:42
#
#-------------------------------------------------

QT       += core gui sql

TARGET = DatabaseManager
TEMPLATE = app


INCLUDEPATH += ../../common/ \
	../../common/connectionWidget/

SOURCES += main.cpp\
        cdatabasemanager.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    ../../common/canimatedstackedwidget.cpp \
    cdatabaseconnectionextwidget.cpp \
    ../../common/global.cpp

HEADERS  += cdatabasemanager.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    ../../common/canimatedstackedwidget.h \
    cdatabaseconnectionextwidget.h \
    ../../common/global.h

FORMS    += cdatabasemanager.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc
