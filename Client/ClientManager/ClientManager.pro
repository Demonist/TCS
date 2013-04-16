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
		../../common/ticketIdentifier/ \
		../../common/aboutDialog/ \
		startingDialog/ \
		clientActionsWidget/ \
		clientRegistrationWidget/ \
		returnTicketWidget/ \
		clientReservationWidget/

SOURCES += main.cpp\
        cclientmainwindow.cpp \
	../../common/global.cpp \
    ../../common/commonWidget/cabstractcommonwidget.cpp \
    ../../common/connectionWidget/cdatabaseconnectionwidget.cpp \
    clientActionsWidget/cclientactionswidget.cpp \
    ../../common/dateDialog/cdatedialog.cpp \
    ../../common/graphics/cgraphicsscene.cpp \
    ../../common/graphics/cgraphicsview.cpp \
    ../../common/cimages.cpp \
    ../../common/graphics/cabstractgraphicsitem.cpp \
    ../../common/canimatedstackedwidget.cpp \
    clientRegistrationWidget/cclientregistrationwidget.cpp \
	clientActionsWidget/cclientactionseatitem.cpp \
    ../../common/graphics/cseatitem.cpp \
    clientActionsWidget/cclientactiondialog.cpp \
    ../../common/graphics/cabstractlegend.cpp \
    ../../common/ticketIdentifier/cticketidentifier.cpp \
    clientActionsWidget/csourcegraphicsview.cpp \
    ../../common/ticketIdentifier/ean13.cpp \
    returnTicketWidget/creturnticketwidget.cpp \
    ../../common/cstatistics.cpp \
    cmarket.cpp \
    clientReservationWidget/cclientreservationwidget.cpp \
    startingDialog/cstartingdialog.cpp \
    clientActionsWidget/cadvertisementitem.cpp \
    ../../common/ccachechecker.cpp \
    ../../common/aboutDialog/caboutdialog.cpp


HEADERS  += cclientmainwindow.h \
	../../common/global.h \
    ../../common/commonWidget/cabstractcommonwidget.h \
    ../../common/connectionWidget/cdatabaseconnectionwidget.h \
    clientActionsWidget/cclientactionswidget.h \
    ../../common/dateDialog/cdatedialog.h \
    ../../common/graphics/cgraphicsscene.h \
    ../../common/graphics/cgraphicsview.h \
    ../../common/cimages.h \
    ../../common/graphics/cabstractgraphicsitem.h \
    ../../common/canimatedstackedwidget.h \
	clientRegistrationWidget/cclientregistrationwidget.h \
    clientActionsWidget/cclientactionseatitem.h \
    ../../common/graphics/cseatitem.h \
    clientActionsWidget/cclientactiondialog.h \
    ../../common/graphics/cabstractlegend.h \
    ../../common/ticketIdentifier/cticketidentifier.h \
    clientActionsWidget/csourcegraphicsview.h \
    ../../common/ticketIdentifier/ean13.h \
    returnTicketWidget/creturnticketwidget.h \
    ../../common/cstatistics.h \
    cmarket.h \
    clientReservationWidget/cclientreservationwidget.h \
    startingDialog/cstartingdialog.h \
    clientActionsWidget/cadvertisementitem.h \
    ../../common/ccachechecker.h \
    ../../common/aboutDialog/caboutdialog.h


FORMS    += cclientmainwindow.ui \
    ../../common/connectionWidget/cdatabaseconnectionwidget.ui \
    clientActionsWidget/cclientactionswidget.ui \
    ../../common/dateDialog/cdatedialog.ui \
	clientRegistrationWidget/cclientregistrationwidget.ui \
    clientActionsWidget/cclientactiondialog.ui \
    returnTicketWidget/creturnticketwidget.ui \
    clientReservationWidget/cclientreservationwidget.ui \
    startingDialog/cstartingdialog.ui \
    ../../common/aboutDialog/caboutdialog.ui

RESOURCES += \
    ../../common/connectionWidget/connectionwidget_res.qrc \
    clinet_res.qrc \
    ../../Admin/MainManager/main_res.qrc \
    ../../common/aboutDialog/aboutDialog_res.qrc

win32 {
	RC_FILE = AppIcon.rc
}
