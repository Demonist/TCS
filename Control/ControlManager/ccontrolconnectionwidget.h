#ifndef CCONTROLCONNECTIONWIDGET_H
#define CCONTROLCONNECTIONWIDGET_H

#include <QtGui>
#include "cdatabaseconnectionwidget.h"

class CControlConnectionWidget : public CDataBaseConnectionWidget
{
	Q_OBJECT

protected:
	QString selectDatabaseFile();

public:
	explicit CControlConnectionWidget(QWidget *parent = 0);
};

#endif // CCONTROLCONNECTIONWIDGET_H
