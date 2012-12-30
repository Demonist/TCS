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
				   "name                 TEXT NULL "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Categories( "
				   "id                   INTEGER " + autoincExpr +
				   "name                 TEXT NULL "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS Places( "
				   "id                   INTEGER " + autoincExpr +
				   "title                 TEXT NULL, "
				   "address               TEXT NULL  "
				   ");"))
		qDebug(qPrintable(query.lastError().text()));

	if(!query.exec("CREATE TABLE IF NOT EXISTS PlaceSchemes( "
				   "id                   INTEGER " + autoincExpr +
				   "seatNumber           INTEGER NULL, "
				   "x                    FLOAT NULL, "
				   "y                    FLOAT NULL, "
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
}

//public:

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);  ////Чтобы не менять в ui.

	ui->connectionWidget->setConnectionName("dataBase");
	connect(ui->connectionWidget, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->connectionWidget, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

	////    ui->connectionWidget->setConnectionChoiceEnable(false);

	ui->sSettings->setCollapsible(0, true);
	ui->sSettings->setStretchFactor(0, 1);
	ui->sSettings->setStretchFactor(1, 100);

	////for testing only:
	if(QFile::exists("test.db") == false)
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test");
		db.setDatabaseName("test.db");
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
}

void MainWindow::connected(QString connectionName)
{
	mConnectionName = connectionName;
	ui->wUsers->setConnectionName(mConnectionName);
	ui->wCategories->setConnectionName(mConnectionName);
	ui->wPlaces->setConnectionName(mConnectionName);
	ui->wClients->setConnectionName(mConnectionName);

	createTables();

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
	}

	ui->swSettings->setCurrentIndex(currentRow);
}
