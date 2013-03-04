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

SOURCES += main.cpp\
        controlmanagermainwindow.cpp \
    ../../common/ticketIdentifier/cticketidentifier.cpp \
    ../../common/ticketIdentifier/ean13.cpp \

HEADERS  += controlmanagermainwindow.h \
    ../../common/ticketIdentifier/cticketidentifier.h \
    ../../common/ticketIdentifier/ean13.h \

FORMS    += controlmanagermainwindow.ui
