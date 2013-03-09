#include "mainwindow.h"
#include "ui_mainwindow.h"

//private:

/**
Функция создает таблицы в базе данных текущего подключения.

Таблицы создаются, только если их нет в базе данных.
Идентификатор подключения берется из \c mConnectionName.
*/
void MainWindow::createTables()
{
	QSqlDatabase db = QSqlDatabase::database(mConnectionName);
	QSqlQuery query(db);
	const bool isServer = db.driverName() == "QMYSQL";
	const QString autoincExpr = isServer ? "AUTO_INCREMENT PRIMARY KEY, " : "PRIMARY KEY AUTOINCREMENT, ";   //Выражение автоинкремента меняется в зависимости от драйвера базы днных.

	//tables:

	if(!query.exec("CREATE TABLE IF NOT EXISTS Users( "
				   "id                   INTEGER " + autoincExpr +
				   "login                TEXT NULL, "
				   "passwordCrypt        TEXT NULL, "
				   "name                 TEXT NULL, "
				   "id_market            INTEGER NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Admins( "
				   "id                   INTEGER " + autoincExpr +
				   "login                TEXT NULL, "
				   "passwordCrypt        TEXT NULL, "
				   "name                 TEXT NULL "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Categories( "
				   "id                   INTEGER " + autoincExpr +
				   "name                 TEXT NULL "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Places( "
				   "id                    INTEGER " + autoincExpr +
				   "title                 TEXT NULL, "
				   "address               TEXT NULL, "
				   "id_background         INTEGER NULL, "
				   "backgroundWidth       INTEGER DEFAULT 400, "
				   "backgroundHeight      INTEGER DEFAULT 300"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS PlaceSchemes( "
				   "id                   INTEGER " + autoincExpr +
				   "seatNumber           TEXT NULL, "
				   "row                  TEXT NULL, "
				   "x                    INTEGER NULL, "
				   "y                    INTEGER NULL, "
				   "id_place             INTEGER NOT NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Clients( "
				   "id                   INTEGER " + autoincExpr +
				   "name                 TEXT NULL, "
				   "birthDate            DATE NULL,  "
				   "login                TEXT NULL,  "
                   "passwordHash         TEXT NULL,  "
				   "phone                TEXT NULL  "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Actions( "
				   "id                   INTEGER " + autoincExpr +
				   "title                TEXT NULL, "
				   "performer            TEXT NULL, "
				   "description          TEXT NULL, "
				   "dateTime             DATETIME NULL, "
				   "state                INTEGER DEFAULT 0, "
				   "fanPrice             INTEGER DEFAULT 0, "
				   "fanPenalty           INTEGER DEFAULT 0, "
				   "fanCount             INTEGER DEFAULT 0, "
				   "id_place             INTEGER NOT NULL, "
				   "id_category          INTEGER NULL, "
				   "id_ticketSubstrate   INTEGER NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS ActionScheme( "
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NOT NULL, "
				   "state				 INTEGER DEFAULT 0, "
				   "id_priceGroup        INTEGER NULL, "
				   "PRIMARY KEY (id_action, id_placeScheme)"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS ActionPriceGroups( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "name                 TEXT NULL, "
				   "price                INTEGER DEFAULT 0, "
				   "penalty              INTEGER DEFAULT 0, "
				   "color                TEXT NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Markets( "
				   "id                   INTEGER " + autoincExpr +
				   "address              TEXT NULL "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Images( "
				   "id                   INTEGER " + autoincExpr +
				   "image                MEDIUMBLOB NULL, "
				   "dateTime             DATETIME NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Tickets( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NULL, "
				   "id_client            INTEGER NULL, "
				   "identifier           TEXT NOT NULL, "
				   "id_market            INTEGER NULL, "
				   "id_user              INTEGER NULL, "
				   "price                INTEGER DEFAULT 0, "
				   "dateTime             DATETIME NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS ReturnedTickets( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "id_client            INTEGER NULL, "
				   "identifier           TEXT NOT NULL, "
				   "id_market            INTEGER NULL, "
				   "id_user              INTEGER NULL, "
				   "penalty              INTEGER DEFAULT 0, "
				   "dateTime             DATETIME NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Reservations( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NULL, "
				   "id_client            INTEGER NULL, "
				   "dateTime             DATETIME NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Statistics( "
				   "id                   INTEGER " + autoincExpr +
				   "type                 INTEGER NOT NULL, "
				   "text                 TEXT NULL, "
				   "data                 BLOB NULL, "
				   "dateTime             DATETIME NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS ComplitedActions( "
				   "id                   INTEGER " + autoincExpr +
				   "title                TEXT NULL, "
				   "performer            TEXT NULL, "
				   "place                TEXT NULL, "
				   "category             TEXT NULL, "
				   "ticketsSolded        INTEGER DEFAULT 0, "
				   "ticketsReturned      INTEGER DEFAULT 0, "
				   "soldedBySite         INTEGER DEFAULT 0, "
				   "income               INTEGER DEFAULT 0, "
				   "penalties            INTEGER DEFAULT 0, "
				   "data                 BLOB NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	//triggers:

	const QString nowExpr = isServer ? "NOW()" : "datetime('now')";

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_deleteUsers BEFORE DELETE ON Users"
				   " BEGIN"
				   "	UPDATE Tickets SET id_user = NULL WHERE id_user = OLD.id;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_deleteActions BEFORE DELETE ON Actions"
				   " FOR EACH ROW BEGIN"
				   "	DELETE FROM Tickets WHERE id_action = OLD.id;"
				   "	DELETE FROM ReturnedTickets WHERE id_action = OLD.id;"
				   "	DELETE FROM Reservations WHERE id_action = OLD.id;"
				   "	DELETE FROM ActionScheme WHERE id_action = OLD.id;"
				   "	DELETE FROM ActionPriceGroups WHERE id_action = OLD.id;"
				   "	DELETE FROM Images WHERE id = OLD.id_ticketSubstrate;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_deletePlaces BEFORE DELETE ON Places"
				   " FOR EACH ROW BEGIN"
				   "	DELETE FROM Actions WHERE id_place = OLD.id;"
				   "	DELETE FROM PlaceSchemes WHERE id_place = OLD.id;"
				   "	DELETE FROM Images WHERE id = OLD.id_background;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_insertTickets AFTER INSERT ON Tickets"
				   " FOR EACH ROW BEGIN"
				   "	UPDATE Tickets SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_insertReturnedTickets AFTER INSERT ON ReturnedTickets"
				   " FOR EACH ROW BEGIN"
				   "	UPDATE ReturnedTickets SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TRIGGER IF NOT EXISTS on_insertStatistics AFTER INSERT ON Statistics"
				   " FOR EACH ROW BEGIN"
				   "	UPDATE Statistics SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
				   " END"
				   ";"))
		qDebug(qPrintable(query.lastError().text()));
}

//public:

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->swSettings->setCurrentIndex(0);

	ui->sSettings->setCollapsible(0, true);
	ui->sSettings->setStretchFactor(0, 1);
	ui->sSettings->setStretchFactor(1, 100);

	////for testing only:
	QString testDbFileName = Global::currentPath() + "/test.db";
	if(QFile::exists(testDbFileName) == false)
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test");
		db.setDatabaseName(testDbFileName);
		if(db.open())
		{
			QSqlQuery query(db);
			if(!query.exec("CREATE TABLE test(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NULL);"))
				qDebug(qPrintable(query.lastError().text()));
			db.close();
		}
	}
	QSqlDatabase::removeDatabase("test");

	mConnectionName = "dataBaseAdmin";
	CAdminStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();
	if(startingDialog.isLogined())
	{
		ui->wUsers->setConnectionName(mConnectionName);
		ui->wCategories->setConnectionName(mConnectionName);
		ui->wPlaces->setConnectionName(mConnectionName);
		ui->wClients->setConnectionName(mConnectionName);
		ui->wActions->setConnectionName(mConnectionName);
		ui->wMarkets->setConnectionName(mConnectionName);
		ui->wAccounting->setConnectionName(mConnectionName);
		ui->wUploading->setConnectionName(mConnectionName);

		createTables();
		CImages::instance(mConnectionName);

		showMaximized();
	}
	else
		QTimer::singleShot(0, this, SLOT(close()));	//Обычный вызов close(); не срабатывает т.к. eventLoop еще не запущен.
}

MainWindow::~MainWindow()
{
	delete ui;

	{
		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		db.close();
	}
	QSqlDatabase::removeDatabase(mConnectionName);
}

void MainWindow::on_lwSettings_currentRowChanged(int currentRow)
{
	switch(currentRow)
	{
		case 0: ui->wUsers->updateData(); break;
		case 1: ui->wCategories->updateData(); break;
		case 2: ui->wPlaces->updateData(); break;
		case 3: ui->wClients->updateData(); break;
		case 4: ui->wActions->updateData(); break;
		case 5: ui->wMarkets->updateData(); break;
		case 6: ui->wAccounting->updateData(); break;
		case 7: ui->wUploading->updateData(); break;
	}

	ui->swSettings->setCurrentIndexAnimatedVertical(currentRow);
}
