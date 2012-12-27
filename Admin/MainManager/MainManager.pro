#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T20:05:39
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainManager
TEMPLATE = app

INCLUDEPATH += ../../common/connectionWidget/ \
	commonWidget/ \
	usersWidget/

SOURCES += main.cpp\
        mainwindow.cpp \
	../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
	commonWidget/cabstractcommonwidget.cpp \
	usersWidget/cuserswidget.cpp \
    usersWidget/cuserdialog.cpp


HEADERS  += mainwindow.h \
	../../common/connectionWidget/cdatabaseconnectionwidget.h \
	commonWidget/cabstractcommonwidget.h \
	usersWidget/cuserswidget.h \
    usersWidget/cuserdialog.h


FORMS    += mainwindow.ui \
	../../common/connectionWidget/cdatabaseconnectionwidget.ui \
	usersWidget/cuserswidget.ui \
    usersWidget/cuserdialog.ui

RESOURCES += \
	../../common/connectionWidget/connectionwidget_res.qrc \
    main_res.qrc
