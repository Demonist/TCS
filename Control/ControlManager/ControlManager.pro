 #-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T09:59:12
#
#-------------------------------------------------

QT       += core gui sql

TARGET = ControlManager
TEMPLATE = app

INCLUDEPATH += ../../common/ \
                ../../common/ticketIdentifier/ \
                ../../common/commonWidget/ \
                ../../common/connectionWidget/ \

SOURCES += main.cpp\
        controlmanagermainwindow.cpp \
    ../../common/ticketIdentifier/cticketidentifier.cpp \
    ../../common/ticketIdentifier/ean13.cpp \
    ../../common/global.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    ccontrolupdatedbase.cpp \
    ../../common/canimatedstackedwidget.cpp

HEADERS  += controlmanagermainwindow.h \
    ../../common/global.h \
    ../../common/ticketIdentifier/cticketidentifier.h \
    ../../common/ticketIdentifier/ean13.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    ccontrolupdatedbase.h \
    ../../common/canimatedstackedwidget.h

FORMS    += controlmanagermainwindow.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui \
    ccontrolupdatedbase.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc
