#include "uploading.h"

#define UPLOADING "uploadingConnection"

Uploading::Uploading(QString mCon, QObject *parent) :
	QObject(parent)
{
	mConnection = mCon;
}

void Uploading::createDBScheme()
{
	QSqlDatabase db = QSqlDatabase::database(UPLOADING);
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

bool Uploading::openConnection(const QString &databasefileName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", UPLOADING);
	db.setDatabaseName(databasefileName);
	if(db.isValid() && db.open())
		return true;
	return false;
}

void Uploading::uploadingProcess(const QString &actionId)
{
	{
		QProgressDialog progressDialog;
		progressDialog.setWindowModality(Qt::WindowModal);
		progressDialog.setWindowTitle(tr("Выгрузка базы данных"));
		progressDialog.setLabelText(tr("Процесс выгрузки базы данных"));
		progressDialog.setCancelButton(0);
		progressDialog.setMinimumDuration(0);
		progressDialog.setMaximum(6);
		progressDialog.setValue(0);

		QSqlDatabase db = QSqlDatabase::database(UPLOADING);
		createDBScheme();

		progressDialog.setValue(1);
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

		QSqlQuery selectDataQuery(QSqlDatabase::database(mConnection));
		QSqlQuery insertDataQuery(db);

		if(selectDataQuery.exec("SELECT id, title, performer FROM Actions WHERE id = " + actionId))
		{
			insertDataQuery.prepare("INSERT INTO Actions VALUES(:id, :title, :performer);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id", selectDataQuery.value(0));
				insertDataQuery.bindValue(":title", selectDataQuery.value(1));
				insertDataQuery.bindValue(":performer", selectDataQuery.value(2));
				insertDataQuery.exec();
			}
		}

		progressDialog.setValue(2);

		QList<int> placeSchemeIds;
		QList<int> clientIds;

		if(selectDataQuery.exec("SELECT id, id_action, id_placeScheme, id_client, identifier FROM Tickets WHERE id_action = " + actionId))
		{
			insertDataQuery.prepare("INSERT INTO Tickets VALUES(NULL, :id_action, :id_placeScheme, :id_client, :identifier, :passedFlag);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id_action", selectDataQuery.value(1));
				insertDataQuery.bindValue(":id_placeScheme", selectDataQuery.value(2));
				insertDataQuery.bindValue(":id_client", selectDataQuery.value(3));
				insertDataQuery.bindValue(":identifier", selectDataQuery.value(4));
				insertDataQuery.bindValue(":passedFlag", "false");
				insertDataQuery.exec();

				if(selectDataQuery.isNull(2) == false)
					placeSchemeIds.append(selectDataQuery.value(2).toInt());

				if(selectDataQuery.isNull(3) == false)
					clientIds.append(selectDataQuery.value(4).toInt());
			}
		}

		progressDialog.setValue(3);

		if(placeSchemeIds.isEmpty() == false)
		{
			if(selectDataQuery.exec("SELECT id, seatNumber, row FROM PlaceSchemes"))
			{
				insertDataQuery.prepare("INSERT INTO PlaceSchemes VALUES(:id, :seatNumber, :row)");
				while(selectDataQuery.next())
				{
					if(placeSchemeIds.contains( selectDataQuery.value(0).toInt()) )
					{
						insertDataQuery.bindValue(":id", selectDataQuery.value(0));
						insertDataQuery.bindValue(":seatNumber", selectDataQuery.value(1));
						insertDataQuery.bindValue(":row", selectDataQuery.value(2));
						insertDataQuery.exec();
					}
				}
			}
		}

		progressDialog.setValue(4);

		if(clientIds.isEmpty() == false)
		{
			if(selectDataQuery.exec("SELECT id, name, login FROM Clients"))
			{
				insertDataQuery.prepare("INSERT INTO Clients VALUES(:id, :name, :login)");
				while(selectDataQuery.next())
				{
					if(clientIds.contains( selectDataQuery.value(0).toInt() ))
					{
						insertDataQuery.bindValue(":id", selectDataQuery.value(0));
						insertDataQuery.bindValue(":name", selectDataQuery.value(1));
						insertDataQuery.bindValue(":login", selectDataQuery.value(2));
						insertDataQuery.exec();
					}
				}
			}
		}

		progressDialog.setValue(5);

		if(selectDataQuery.exec("SELECT id, id_client, identifier FROM ReturnedTickets WHERE id_action = " + actionId))
		{
			insertDataQuery.prepare("INSERT INTO ReturnedTickets VALUES(NULL, :id_client, :identifier);");
			while(selectDataQuery.next())
			{
				insertDataQuery.bindValue(":id_client", selectDataQuery.value(1));
				insertDataQuery.bindValue(":identifier", selectDataQuery.value(2));
				insertDataQuery.exec();
			}
		}

		progressDialog.setValue(6);
		progressDialog.close();
		db.close();
	}
	QSqlDatabase::removeDatabase(UPLOADING);
}


