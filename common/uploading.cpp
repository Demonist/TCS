#include "uploading.h"

Uploading::Uploading(QString mCon, QString pth, QString id_action, QObject *parent) :
	QObject(parent)
{
	mConnection = mCon;
	mPath = pth;
	mIDAction = id_action;
}

void Uploading::createDBScheme()
{
	QSqlDatabase db = QSqlDatabase::database("uploadingConnection");
	QSqlQuery createDBQuery(db);
	if(!createDBQuery.exec(
				"CREATE TABLE IF NOT EXISTS Actions( "
				"id                   INTEGER NOT NULL, "
				"title                TEXT NULL, "
				"performer            TEXT NULL "
				"); "
				))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec(
				"CREATE TABLE IF NOT EXISTS Tickets( "
				"id                   INTEGER PRIMARY KEY AUTOINCREMENT, "
				"id_action            INTEGER NULL, "
				"id_placeScheme       INTEGER NULL, "
				"id_client            INTEGER NULL, "
				"identifier           TEXT NULL, "
				"passedFlag           TEXT NOT NULL "
				");"
				))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec(
				"CREATE TABLE IF NOT EXISTS Clients( "
				"id                   INTEGER NOT NULL, "
				"name                 TEXT NULL, "
				"login                TEXT NULL "
				");"
				))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec("CREATE TABLE IF NOT EXISTS ReturnedTickets( "
						   "id                   INTEGER PRIMARY KEY AUTOINCREMENT, "
						   "id_client            INTEGER NULL, "
						   "identifier           TEXT NOT NULL "
						   ");"))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec("CREATE TABLE IF NOT EXISTS PlaceSchemes( "
						   "id                   INTEGER NOT NULL, "
						   "seatNumber           INTEGER NULL, "
						   "row					 INTEGER NULL "
						   ");"))
		qDebug(qPrintable(createDBQuery.lastError().text()));
}

bool Uploading::openConnection()
{
	//if(validateDataBase())
	//{
		//имя подключения в define
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "uploadingConnection");
		db.setDatabaseName(mPath);
		if(db.open())
		{
			return true;
		}
		else
		{
			return false;
		}
	/*}
	else
	{
		return false;
	}*/
}

/*bool Uploading::validateDataBase()
{
	QSqlQuery vQuery(QSqlDatabase::database(mConnection));
	vQuery.exec("SELECT COUNT(id) AS mCount FROM Tickets WHERE id_action = " + mIDAction + " GROUP BY identifier ORDER BY mCount DESC");
	vQuery.first();
	if(vQuery.value(0).toInt() > 1)
	{
		return false;
	}
	else
	{
		return true;
	}

}*/

void Uploading::uploadingProcess()
{
	{
		QProgressDialog dlg;
		dlg.setWindowTitle(tr("Выгрузка базы данных"));
		dlg.setLabelText(tr("Процесс выгрузки базы данных"));
		dlg.setCancelButton(0);
		dlg.setMinimumDuration(0);
		dlg.setMaximum(3);
		dlg.show();
		QSqlDatabase db = QSqlDatabase::database("uploadingConnection");
		createDBScheme();
		QSqlQuery selectDataQuery(QSqlDatabase::database(mConnection));
		if(selectDataQuery.exec("SELECT id, title, performer FROM Actions WHERE id = " + mIDAction))
		{
			QSqlQuery insertDataQuery(db);
			insertDataQuery.exec("INSERT INTO Actions VALUES(:id, :title, :performer);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id", selectDataQuery.value(0).toString());
				insertDataQuery.bindValue(":title", selectDataQuery.value(1).toString());
				insertDataQuery.bindValue(":performer", selectDataQuery.value(2).toString());
				insertDataQuery.exec();

			}
		}
		dlg.setValue(1);
		if(selectDataQuery.exec("SELECT id, id_action, id_placeScheme, id_client, identifier FROM Tickets WHERE id_action = " + mIDAction))
		{
			QSqlQuery insertDataQuery(db);
			insertDataQuery.exec("INSERT INTO Tickets VALUES(NULL, :id_action, :id_placeScheme, :id_client, :identifier, :passedFlag);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id_action", selectDataQuery.value(1).toString());
				insertDataQuery.bindValue(":id_placeScheme", selectDataQuery.value(2).toString());
				insertDataQuery.bindValue(":id_client", selectDataQuery.value(3).toString());
				insertDataQuery.bindValue(":identifier", selectDataQuery.value(4).toString());
				insertDataQuery.bindValue(":passedFlag", "false");
				insertDataQuery.exec();

				QSqlQuery placeScheme(QSqlDatabase::database(mConnection));
				if(placeScheme.exec("SELECT id, seatNumber, row FROM PlaceSchemes WHERE id = " + selectDataQuery.value(2).toString()))
				{
					QSqlQuery insertScheme(db);
					insertScheme.exec("INSERT INTO PlaceSchemes VALUES(:id, :seatNumber, :row)");
					while(placeScheme.next())
					{
						insertScheme.bindValue(":id", placeScheme.value(0).toString());
						insertScheme.bindValue(":seatNumber", placeScheme.value(1).toString());
						insertScheme.bindValue(":row", placeScheme.value(2).toString());
						insertScheme.exec();
					}
				}

				if(!selectDataQuery.value(3).isNull())
				{
					QSqlQuery selectDataClients(QSqlDatabase::database(mConnection));
					if(selectDataClients.exec("SELECT id, name, login FROM Clients WHERE id = " + selectDataQuery.value(3).toString()))
					{
						QSqlQuery insertDataClients(db);
						insertDataClients.exec("INSERT INTO Clients VALUES(:id, :name, :login)");
						while(selectDataClients.next())
						{
							insertDataClients.bindValue(":id", selectDataClients.value(0).toString());
							insertDataClients.bindValue(":name", selectDataClients.value(1).toString());
							insertDataClients.bindValue(":login", selectDataClients.value(2).toString());
							insertDataClients.exec();
						}
					}
				}
			}
		}
		dlg.setValue(2);
		if(selectDataQuery.exec("SELECT id, id_client, identifier FROM ReturnedTickets WHERE id_action = " + mIDAction))
		{
			QSqlQuery insertDataQuery(db);
			insertDataQuery.exec("INSERT INTO ReturnedTickets VALUES(NULL, :id_client, :identifier);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id_client", selectDataQuery.value(1).toString());
				insertDataQuery.bindValue(":identifier", selectDataQuery.value(2).toString());
				insertDataQuery.exec();
			}
		}
		dlg.setValue(3);
		dlg.close();
		db.close();
	}
	QSqlDatabase::removeDatabase("uploadingConnection");
}


