#-------------------------------------------------
#
# Project created by QtCreator 2012-12-26T20:05:39
#
#-------------------------------------------------

QT       += core gui sql

TARGET = MainManager
TEMPLATE = app

INCLUDEPATH += ../../common/ \
		../../common/commonWidget/ \
		../../common/connectionWidget/ \
		../../common/graphics/ \
		../../common/dateDialog/ \
        usersWidget/ \
        categoriesWidget/ \
        placesWidget/ \
		clientsWidget/ \
		actionsWidget/ \
		marketsWidget/ \
        accountingWidget/ \
		uploadingWidget/ \
		imageDialog/

SOURCES += main.cpp\
        mainwindow.cpp \
	../../common/global.cpp \
	../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
	usersWidget/cuserswidget.cpp \
    usersWidget/cuserdialog.cpp \
    categoriesWidget/ccategorieswidget.cpp \
    categoriesWidget/ccategorydialog.cpp \
    placesWidget/cplaceswidget.cpp \
    placesWidget/cplacedialog.cpp \
    clientsWidget/cclientdialog.cpp \
    clientsWidget/cclientswidget.cpp \
    placesWidget/cplaceschemedialog.cpp \
	actionsWidget/cactionswidget.cpp \
	actionsWidget/cactiondialog.cpp \
    ../../common/dateDialog/cdatedialog.cpp \
    actionsWidget/cactionticketsmanagement.cpp \
	marketsWidget/cmarketswidget.cpp \
	marketsWidget/cmarketdialog.cpp \
    placesWidget/cdistanceitem.cpp \
    ../../common/cimages.cpp \
    accountingWidget/caccountingwidget.cpp \
    accountingWidget/caccountingactionsdialog.cpp \
    uploadingWidget/cuploadingwidget.cpp \
    ../../common/commonWidget/cabstractcommonwidget.cpp \
    ../../common/graphics/cseatitem.cpp \
    ../../common/graphics/cactionseatitem.cpp \
    ../../common/graphics/cgraphicsscene.cpp \
    ../../common/graphics/cgraphicsview.cpp \
    ../../common/graphics/cabstractlegend.cpp \
    actionsWidget/cpricedialog.cpp \
    imageDialog/cimagedialog.cpp \
    ../../common/graphics/cabstractgraphicsitem.cpp


HEADERS  += mainwindow.h \
	../../common/global.h \
	../../common/connectionWidget/cdatabaseconnectionwidget.h \
	usersWidget/cuserswidget.h \
    usersWidget/cuserdialog.h \
    categoriesWidget/ccategorieswidget.h \
    categoriesWidget/ccategorydialog.h \
    placesWidget/cplaceswidget.h \
    placesWidget/cplacedialog.h \
    clientsWidget/cclientdialog.h \
    clientsWidget/cclientswidget.h \
    placesWidget/cplaceschemedialog.h \
	actionsWidget/cactionswidget.h \
    actionsWidget/cactiondialog.h \
    ../../common/dateDialog/cdatedialog.h \
    actionsWidget/cactionticketsmanagement.h \
    ../../common/parallel.h \
	marketsWidget/cmarketswidget.h \
	marketsWidget/cmarketdialog.h \
    placesWidget/cdistanceitem.h \
    ../../common/cimages.h \
    accountingWidget/caccountingwidget.h \
    accountingWidget/caccountingactionsdialog.h \
    uploadingWidget/cuploadingwidget.h \
    ../../common/commonWidget/cabstractcommonwidget.h \
    ../../common/graphics/cseatitem.h \
    ../../common/graphics/cactionseatitem.h \
    ../../common/graphics/cgraphicsscene.h \
    ../../common/graphics/cgraphicsview.h \
    ../../common/graphics/cabstractlegend.h \
    actionsWidget/cpricedialog.h \
    imageDialog/cimagedialog.h \
    ../../common/graphics/cabstractgraphicsitem.h

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
	marketsWidget/cmarketdialog.ui \
    accountingWidget/caccountingwidget.ui \
    accountingWidget/caccountingactionsdialog.ui \
    uploadingWidget/cuploadingwidget.ui \
    actionsWidget/cpricedialog.ui \
    imageDialog/cimagedialog.ui

RESOURCES += \
	../../common/connectionWidget/connectionwidget_res.qrc \
    main_res.qrc \
    placesWidget/placeWidget_res.qrc \
    actionsWidget/actionWidget_res.qrc
