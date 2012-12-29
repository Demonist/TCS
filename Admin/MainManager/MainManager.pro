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
        usersWidget/ \
        categoriesWidget/ \
        placesWidget/ \
        clientsWidget/

SOURCES += main.cpp\
        mainwindow.cpp \
	../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
	commonWidget/cabstractcommonwidget.cpp \
	usersWidget/cuserswidget.cpp \
    usersWidget/cuserdialog.cpp \
    categoriesWidget/ccategorieswidget.cpp \
    categoriesWidget/ccategorydialog.cpp \
    placesWidget/cplaceswidget.cpp \
    placesWidget/cplacedialog.cpp \
    clientsWidget/cclientdialog.cpp \
    clientsWidget/cclientswidget.cpp


HEADERS  += mainwindow.h \
	../../common/connectionWidget/cdatabaseconnectionwidget.h \
	commonWidget/cabstractcommonwidget.h \
	usersWidget/cuserswidget.h \
    usersWidget/cuserdialog.h \
    categoriesWidget/ccategorieswidget.h \
    categoriesWidget/ccategorydialog.h \
    placesWidget/cplaceswidget.h \
    placesWidget/cplacedialog.h \
    clientsWidget/cclientdialog.h \
    clientsWidget/cclientswidget.h


FORMS    += mainwindow.ui \
	../../common/connectionWidget/cdatabaseconnectionwidget.ui \
	usersWidget/cuserswidget.ui \
    usersWidget/cuserdialog.ui \
    categoriesWidget/ccategorieswidget.ui \
    categoriesWidget/ccategorydialog.ui \
    placesWidget/cplaceswidget.ui \
    placesWidget/cplacedialog.ui \
    clientsWidget/cclientdialog.ui \
    clientsWidget/cclientswidget.ui

RESOURCES += \
	../../common/connectionWidget/connectionwidget_res.qrc \
    main_res.qrc
