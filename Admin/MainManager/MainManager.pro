#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T20:05:39
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainManager
TEMPLATE = app

INCLUDEPATH += ../../common/ \
		../../common/connectionWidget/ \
		../../common/seatItem/ \
		../../common/dateDialog/ \
        commonWidget/ \
        usersWidget/ \
        categoriesWidget/ \
        placesWidget/ \
		clientsWidget/ \
		actionsWidget/ \
		marketsWidget/

SOURCES += main.cpp\
        mainwindow.cpp \
	../../common/global.cpp \
	../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
	commonWidget/cabstractcommonwidget.cpp \
	usersWidget/cuserswidget.cpp \
    usersWidget/cuserdialog.cpp \
    categoriesWidget/ccategorieswidget.cpp \
    categoriesWidget/ccategorydialog.cpp \
    placesWidget/cplaceswidget.cpp \
    placesWidget/cplacedialog.cpp \
    clientsWidget/cclientdialog.cpp \
    clientsWidget/cclientswidget.cpp \
    placesWidget/cplaceschemedialog.cpp \
    ../../common/seatItem/cseatitem.cpp \
	actionsWidget/cactionswidget.cpp \
	actionsWidget/cactiondialog.cpp \
    ../../common/dateDialog/cdatedialog.cpp \
    actionsWidget/cactionticketsmanagement.cpp \
	../../common/seatItem/cactionseatitem.cpp \
	marketsWidget/cmarketswidget.cpp \
	marketsWidget/cmarketdialog.cpp \
    placesWidget/cdistanceitem.cpp


HEADERS  += mainwindow.h \
	../../common/global.h \
	../../common/connectionWidget/cdatabaseconnectionwidget.h \
	commonWidget/cabstractcommonwidget.h \
	usersWidget/cuserswidget.h \
    usersWidget/cuserdialog.h \
    categoriesWidget/ccategorieswidget.h \
    categoriesWidget/ccategorydialog.h \
    placesWidget/cplaceswidget.h \
    placesWidget/cplacedialog.h \
    clientsWidget/cclientdialog.h \
    clientsWidget/cclientswidget.h \
    placesWidget/cplaceschemedialog.h \
    ../../common/seatItem/cseatitem.h \
	actionsWidget/cactionswidget.h \
    actionsWidget/cactiondialog.h \
    ../../common/dateDialog/cdatedialog.h \
    actionsWidget/cactionticketsmanagement.h \
    ../../common/parallel.h \
	../../common/seatItem/cactionseatitem.h \
	marketsWidget/cmarketswidget.h \
	marketsWidget/cmarketdialog.h \
    placesWidget/cdistanceitem.h

FORMS    += mainwindow.ui \
	../../common/connectionWidget/cdatabaseconnectionwidget.ui \
	usersWidget/cuserswidget.ui \
    usersWidget/cuserdialog.ui \
    categoriesWidget/ccategorieswidget.ui \
    categoriesWidget/ccategorydialog.ui \
    placesWidget/cplaceswidget.ui \
    placesWidget/cplacedialog.ui \
    clientsWidget/cclientdialog.ui \
    clientsWidget/cclientswidget.ui \
    placesWidget/cplaceschemedialog.ui \
    actionsWidget/cactionswidget.ui \
    actionsWidget/cactiondialog.ui \
    ../../common/dateDialog/cdatedialog.ui \
	actionsWidget/cactionticketsmanagement.ui \
	marketsWidget/cmarketswidget.ui \
	marketsWidget/cmarketdialog.ui

RESOURCES += \
	../../common/connectionWidget/connectionwidget_res.qrc \
    main_res.qrc \
    placesWidget/placeWidget_res.qrc \
    actionsWidget/actionWidget_res.qrc
