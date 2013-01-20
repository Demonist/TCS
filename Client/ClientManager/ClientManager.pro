#-------------------------------------------------
#
# Project created by QtCreator 2013-01-20T23:08:50
#
#-------------------------------------------------

QT       += core gui sql

TARGET = ClientManager
TEMPLATE = app

INCLUDEPATH += ../../common/ \
		../../common/commonWidget/ \
		../../common/connectionWidget/ \
		../../common/seatItem/ \
		../../common/dateDialog/ \
		startingDialog/

SOURCES += main.cpp\
        cclientmainwindow.cpp \
    ../../common/commonWidget/cabstractcommonwidget.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    startingDialog/cstartingdialog.cpp

HEADERS  += cclientmainwindow.h \
    ../../common/commonWidget/cabstractcommonwidget.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    startingDialog/cstartingdialog.h

FORMS    += cclientmainwindow.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui \
    startingDialog/cstartingdialog.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc \
    clinet_res.qrc
