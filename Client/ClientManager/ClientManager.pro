#-------------------------------------------------
#
# Project created by QtCreator 2013-01-18T15:13:53
#
#-------------------------------------------------

QT       += core gui sql

TARGET = ClientManager
TEMPLATE = app

INCLUDEPATH += ../../common/ \
		../../common/connectionWidget/ \
		../../common/seatItem/ \
		../../common/dateDialog/ \
		startingDialog/

SOURCES += main.cpp\
        mainwindow.cpp \
    startingDialog/cstartingdialog.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    ../../common/global.cpp \
    ../../common/cimages.cpp \
    actionsWidget/cactionswidget.cpp

HEADERS  += mainwindow.h \
    startingDialog/cstartingdialog.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    ../../common/parallel.h \
    ../../common/global.h \
    ../../common/cimages.h \
    actionsWidget/cactionswidget.h

FORMS    += mainwindow.ui \
    startingDialog/cstartingdialog.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui \
    actionsWidget/cactionswidget.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc \
    clinet_res.qrc
