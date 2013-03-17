#ifndef CDATABASECONNECTIONEXTWIDGET_H
#define CDATABASECONNECTIONEXTWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cdatabaseconnectionwidget.h"

class CDatabaseConnectionExtWidget : public CDataBaseConnectionWidget
{
	Q_OBJECT
protected:
	void processConnecting();
	QString selectDatabaseFile();

public:
	explicit CDatabaseConnectionExtWidget(QWidget *parent = 0);

signals:
	void connectedToFile(QString fileName);
	void connectedToHost();
	void connectedToDatabase(QString databaseName);
};

#endif // CDATABASECONNECTIONEXTWIDGET_H
