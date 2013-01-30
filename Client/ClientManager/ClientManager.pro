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
		../../common/graphics/ \
		../../common/dateDialog/ \
		startingDialog/ \
		clientActionsWidget/

SOURCES += main.cpp\
        cclientmainwindow.cpp \
    ../../common/commonWidget/cabstractcommonwidget.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    startingDialog/cstartingdialog.cpp \
    clientActionsWidget/cclientactionswidget.cpp \
    ../../common/dateDialog/cdatedialog.cpp \
    ../../common/graphics/cgraphicsscene.cpp \
    ../../common/graphics/cgraphicsview.cpp \
    ../../common/cimages.cpp \
    ../../common/graphics/cabstractgraphicsitem.cpp \
    ../../common/canimatedstackedwidget.cpp

HEADERS  += cclientmainwindow.h \
    ../../common/commonWidget/cabstractcommonwidget.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    startingDialog/cstartingdialog.h \
    clientActionsWidget/cclientactionswidget.h \
    ../../common/dateDialog/cdatedialog.h \
    ../../common/graphics/cgraphicsscene.h \
    ../../common/graphics/cgraphicsview.h \
    ../../common/cimages.h \
    ../../common/graphics/cabstractgraphicsitem.h \
    ../../common/canimatedstackedwidget.h

FORMS    += cclientmainwindow.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui \
    startingDialog/cstartingdialog.ui \
    clientActionsWidget/cclientactionswidget.ui \
    ../../common/dateDialog/cdatedialog.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc \
    clinet_res.qrc \
    ../../Admin/MainManager/main_res.qrc
