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
				"performer            TEXT NULL, "
				"description          TEXT NULL, "
				"dateTime             DATETIME NULL, "
				"state                INTEGER DEFAULT 0, "
				"fanPrice             INTEGER DEFAULT 0, "
				"fanCount             INTEGER DEFAULT 0, "
				"id_place             INTEGER NULL, "
				"id_category          INTEGER NULL"
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
				"passedFlag           TEXT NOT NULL"
				");"
				))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec(
				"CREATE TABLE IF NOT EXISTS Clients( "
				"id                   INTEGER NOT NULL, "
				"name                 TEXT NULL, "
				"birthDate            DATE NULL,  "
				"login                TEXT NULL,  "
				"passwordHash         TEXT NULL,  "
				"phone                TEXT NULL  "
				");"
				))
		qDebug(qPrintable(createDBQuery.lastError().text()));
	if(!createDBQuery.exec("CREATE TABLE IF NOT EXISTS ReturnedTickets( "
						   "id                   INTEGER PRIMARY KEY AUTOINCREMENT, "
						   "id_action            INTEGER NOT NULL, "
						   "id_client            INTEGER NULL, "
						   "identifier           TEXT NOT NULL, "
						   "id_market            INTEGER NULL, "
						   "id_user              INTEGER NULL, "
						   "penalty              INTEGER DEFAULT 0, "
						   "dateTime             DATETIME NULL "
						   ");"))
		qDebug(qPrintable(createDBQuery.lastError().text()));
}

bool Uploading::openConnection()
{
	if(validateDataBase())
	{
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
	}
	else
	{
		return false;
	}
}

bool Uploading::validateDataBase()
{
	QSqlQuery vQuery(QSqlDatabase::database(mConnection));
	vQuery.exec("SELECT COUNT(*) AS mCount FROM Tickets WHERE id_action = " + mIDAction + " GROUP BY identifier ORDER BY mCount DESC");
	vQuery.first();
	if(vQuery.value(0).toInt() > 1)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void Uploading::uploadingProcess()
{
	{
		QSqlDatabase db = QSqlDatabase::database("uploadingConnection");
		createDBScheme();
		QSqlQuery selectDataQuery(QSqlDatabase::database(mConnection));
		if(selectDataQuery.exec("SELECT * FROM Actions WHERE id = " + mIDAction))
		{
			QSqlQuery insertDataQuery(db);
			insertDataQuery.exec("INSERT INTO Actions VALUES(:id, :title, :performer, :description, :date, :state, :fanPrice, :fanCount, :id_place, :id_cat);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id", selectDataQuery.value(0).toString());
				insertDataQuery.bindValue(":title", selectDataQuery.value(1).toString());
				insertDataQuery.bindValue(":performer", selectDataQuery.value(2).toString());
				insertDataQuery.bindValue(":description", selectDataQuery.value(3).toString());
				insertDataQuery.bindValue(":date", selectDataQuery.value(4).toString());
				insertDataQuery.bindValue(":state", selectDataQuery.value(5).toString());
				insertDataQuery.bindValue(":fanPrice", selectDataQuery.value(6).toString());
				insertDataQuery.bindValue(":fanCount", selectDataQuery.value(7).toString());
				insertDataQuery.bindValue(":id_place", selectDataQuery.value(8).toString());
				insertDataQuery.bindValue(":id_cat", selectDataQuery.value(9).toString());
				insertDataQuery.exec();

			}
		}
		if(selectDataQuery.exec("SELECT * FROM Tickets WHERE id_action = " + mIDAction))
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
				if(!selectDataQuery.value(3).isNull())
				{
					QSqlQuery selectDataClients(QSqlDatabase::database(mConnection));
					if(selectDataClients.exec("SELECT * FROM Clients WHERE id = " + selectDataQuery.value(3).toString()))
					{
						QSqlQuery insertDataClients(db);
						insertDataClients.exec("INSERT INTO Clients VALUES(:id, :name, :birthDate, :login, :passwordHash, :phone)");
						while(selectDataClients.next())
						{
							insertDataClients.bindValue(":id", selectDataClients.value(0).toString());
							insertDataClients.bindValue(":name", selectDataClients.value(1).toString());
							insertDataClients.bindValue(":birthDate", selectDataClients.value(2).toString());
							insertDataClients.bindValue(":login", selectDataClients.value(3).toString());
							insertDataClients.bindValue(":passwordHash", selectDataClients.value(4).toString());
							insertDataClients.bindValue(":phone", selectDataClients.value(5).toString());
							insertDataClients.exec();
						}
					}
				}
			}
		}
		if(selectDataQuery.exec("SELECT * FROM ReturnedTickets WHERE id_action = " + mIDAction))
		{
			QSqlQuery insertDataQuery(db);
			insertDataQuery.exec("INSERT INTO ReturnedTickets VALUES(NULL, :id_action, :id_client, :identifier, :id_market, :id_user, :penalty, :dateTime);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id_action", selectDataQuery.value(1).toString());
				insertDataQuery.bindValue(":id_client", selectDataQuery.value(2).toString());
				insertDataQuery.bindValue(":identifier", selectDataQuery.value(3).toString());
				insertDataQuery.bindValue(":id_market", selectDataQuery.value(4).toString());
				insertDataQuery.bindValue(":id_user", selectDataQuery.value(5).toString());
				insertDataQuery.bindValue(":penalty", selectDataQuery.value(6).toString());
				insertDataQuery.bindValue(":dateTime", selectDataQuery.value(7).toString());
				insertDataQuery.exec();
			}
		}
		db.close();
	}
	QSqlDatabase::removeDatabase("uploadingConnection");
}


