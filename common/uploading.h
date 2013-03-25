#ifndef UPLOADING_H
#define UPLOADING_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"

class Uploading : public QObject
{
	Q_OBJECT
public:
	explicit Uploading(QString mCon, QString pth, QString id_action, QObject *parent = 0);
	void createDBScheme();
	bool openConnection();
	void uploadingProcess();
signals:
	
public slots:

private:
	QString mConnection;
	QString mPath;
	QString mIDAction;
	//bool validateDataBase();

	
};

#endif // UPLOADING_H
