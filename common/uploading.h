#ifndef UPLOADING_H
#define UPLOADING_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"

class Uploading : public QObject
{
	Q_OBJECT

public:
	explicit Uploading(QString mCon, QObject *parent = 0);

	void createDBScheme();
	bool openConnection(const QString &databasefileName);
	void uploadingProcess(const QString &actionId);

private:
	QString mConnection;
};

#endif // UPLOADING_H
