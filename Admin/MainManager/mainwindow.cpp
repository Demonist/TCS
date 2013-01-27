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
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	QString autoincExpr = ui->connectionWidget->connectionType() == CDataBaseConnectionWidget::ConnectionServer ? "AUTO_INCREMENT PRIMARY KEY, " : "PRIMARY KEY AUTOINCREMENT, ";   //Выражение автоинкремента меняется в зависимости от драйвера базы днных.

	if(!query.exec("CREATE TABLE IF NOT EXISTS Users( "
				   "id                   INTEGER " + autoincExpr +
				   "login                TEXT NULL, "
				   "passwordCrypt        TEXT NULL, "
				   "name                 TEXT NULL, "
				   "marketsID            INTEGER NOT NULL"
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
				   "passwordHash         TEXT NULL  "
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
				   "id_place             INTEGER NULL, "
				   "id_category          INTEGER NULL, "
				   "id_logoImage         INTEGER NULL"
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS ActionScheme( "
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NOT NULL, "
				   "state				 INTEGER DEFAULT 0, "
				   "id_priceGroup        INTEGER NULL, "
				   "id_reservation       INTEGER NULL, "
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
}

//public:

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);  ////Чтобы не менять в ui.

	ui->connectionWidget->setConnectionName("dataBaseAdmin");
	connect(ui->connectionWidget, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->connectionWidget, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

	////    ui->connectionWidget->setConnectionChoiceEnable(false);

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

void MainWindow::connected(QString connectionName)
{
	mConnectionName = connectionName;
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

	ui->stackedWidget->setCurrentIndex(1);
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

	ui->swSettings->setCurrentIndex(currentRow);
}
