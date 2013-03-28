#include "cdatabasemanager.h"
#include "ui_cdatabasemanager.h"

//protected:

void CDatabaseManager::showDatabases()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SHOW DATABASES;"))
	{
		ui->lwDatabases->clear();
		while(query.next())
			ui->lwDatabases->addItem(query.value(0).toString());
		ui->gbxDatabases->setTitle(tr("Базы данных: %1 шт.").arg(ui->lwDatabases->count()));
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}

void CDatabaseManager::showTables()
{
	if(ui->wConnectingWidget->connectionType() == CDataBaseConnectionWidget::ConnectionServer)
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		if(query.exec("SHOW TABLES;"))
		{
			ui->lwTables->clear();
			while(query.next())
				ui->lwTables->addItem(query.value(0).toString());
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
	else
	{
		ui->lwTables->clear();
		ui->lwTables->addItems(QSqlDatabase::database(mConnectionName).tables());
	}
	ui->gbxTables->setTitle(tr("Таблицы: %1 шт.").arg(ui->lwTables->count()));
}

void CDatabaseManager::connectToDatabase(const QString &databaseName)
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("USE `" + databaseName + "`;"))
		connectingToDatabase(databaseName);
	else
		qDebug(qPrintable(query.lastError().text()));
}

//public:

CDatabaseManager::CDatabaseManager(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CDatabaseManager)
{
	ui->setupUi(this);
	mConnectionName = "databaseManager";
	ui->stackedWidget->setCurrentIndex(0);
	ui->wConnectingWidget->setConnectionName(mConnectionName);
	connect(ui->wConnectingWidget, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->wConnectingWidget, SIGNAL(connectedToDatabase(QString)), this, SLOT(connectingToDatabase(QString)));
	connect(ui->wConnectingWidget, SIGNAL(connectedToFile(QString)), this, SLOT(connectingToFile(QString)));
	connect(ui->wConnectingWidget, SIGNAL(connectedToHost()), this, SLOT(connectingToHost()));
}

CDatabaseManager::~CDatabaseManager()
{
	delete ui;

	{
		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		db.close();
	}
	QSqlDatabase::removeDatabase(mConnectionName);
}

void CDatabaseManager::connectingToFile(const QString &fileName)
{
	mServer = false;

	QFileInfo fileInfo(fileName);
	QString text;
	if(fileInfo.size() > 0)
		text = tr("Файл базы данных: %1");
	else
		text = tr("Новый файл базы данных: %1");
	ui->lDatabase->setText(text.arg(fileInfo.baseName()));
	ui->lDatabase->setToolTip(fileInfo.absoluteFilePath());

	ui->stackedWidget->setCurrentIndexAnimatedHorizontal(2);
	showTables();
}

void CDatabaseManager::connectingToHost()
{
	mServer = true;

	ui->stackedWidget->setCurrentIndexAnimatedHorizontal(1);

	showDatabases();
}

void CDatabaseManager::connectingToDatabase(const QString &databaseName)
{
	mServer = true;

	ui->lDatabase->setText(tr("База данных: %1").arg(databaseName));

	ui->stackedWidget->setCurrentIndexAnimatedHorizontal(2);
	showTables();
}

void CDatabaseManager::on_pbnConnectToDB_clicked()
{
	QListWidgetItem *selected = ui->lwDatabases->selectedItems().value(0);
	if(selected)
		connectToDatabase(selected->text());
}

void CDatabaseManager::on_pbnDeleteDB_clicked()
{
	QListWidgetItem *selected = ui->lwDatabases->selectedItems().value(0);
	if(selected)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить базу данных '%1'?").arg(selected->text()), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			if(query.exec("DROP DATABASE `" + selected->text() + "`;"))
				showDatabases();
			else
				qDebug(qPrintable(query.lastError().text()));
		}
	}
}

void CDatabaseManager::on_pbnCreateDB_clicked()
{
	const QString &dbName = ui->leNewDBName->text();

	QString error;
	if(dbName.isEmpty())
		error = tr("Укажите имя для новой базы данных.");
	else if(ui->lwDatabases->findItems(dbName, Qt::MatchFixedString | Qt::MatchContains).count() > 0)
		error = tr("База данных с таким именем уже существует");
	if(error.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Внимание"), error);
		return;
	}

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("CREATE DATABASE `" + dbName +"` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"))
		connectToDatabase(dbName);
	else
		qDebug(qPrintable(query.lastError().text()));
}

void CDatabaseManager::on_pbnClear_clicked()
{
	QListWidgetItem *selected = ui->lwTables->selectedItems().value(0);
	if(selected)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите очистить таблицу '%1'?").arg(selected->text()), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			if(mServer)
			{
				if(!query.exec("TRUNCATE TABLE `" + selected->text() + "`;"))
					qDebug(qPrintable(query.lastError().text()));
			}
			else
			{
				if(!query.exec("DELETE FROM`" + selected->text() + "` WHERE 1;"))
					qDebug(qPrintable(query.lastError().text()));
			}
		}
	}
}

void CDatabaseManager::on_pbnDelete_clicked()
{
	QListWidgetItem *selected = ui->lwTables->selectedItems().value(0);
	if(selected)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить таблицу '%1'?").arg(selected->text()), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			if(query.exec("DROP TABLE `" + selected->text() + "`;"))
				showTables();
			else
				qDebug(qPrintable(query.lastError().text()));
		}
	}
}

void CDatabaseManager::on_pbnClearAll_clicked()
{
	if(ui->lwTables->count() == 0)
		return;

	if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите очисттиь все таблицы?"), QMessageBox::Yes, QMessageBox::No))
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		for(int i = 0; i < ui->lwTables->count(); i++)
			if(mServer)
			{
				if(!query.exec("TRUNCATE TABLE `" + ui->lwTables->item(i)->text() + "`;"))
					qDebug(qPrintable(query.lastError().text()));
			}
			else
			{
				if(!query.exec("DELETE FROM `" + ui->lwTables->item(i)->text() + "` WHERE 1;"))
					qDebug(qPrintable(query.lastError().text()));
			}
	}
}

void CDatabaseManager::on_pbnDeleteAll_clicked()
{
	if(ui->lwTables->count() == 0)
		return;

	if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить все таблицы?"), QMessageBox::Yes, QMessageBox::No))
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		for(int i = 0; i < ui->lwTables->count(); i++)
			if(!query.exec("DROP TABLE `" + ui->lwTables->item(i)->text() + "`;"))
				qDebug(qPrintable(query.lastError().text()));

		showTables();
	}
}

void CDatabaseManager::on_pbnCreateTables_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	const QString autoincExpr = mServer ? "AUTO_INCREMENT PRIMARY KEY, " : "PRIMARY KEY AUTOINCREMENT, ";   //Выражение автоинкремента меняется в зависимости от драйвера базы днных.
	const QString tableTypeExpr = mServer ? ") ENGINE=InnoDB DEFAULT CHARSET=utf8;" : ");";

	//tables:

	QString tablesError;

	if(!query.exec("CREATE TABLE IF NOT EXISTS Users( "
				   "id                   INTEGER " + autoincExpr +
				   "login                TEXT NULL, "
				   "passwordCrypt        TEXT NULL, "
				   "name                 TEXT NULL, "
				   "id_market            INTEGER NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Users\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Admins( "
				   "id                   INTEGER " + autoincExpr +
				   "login                TEXT NULL, "
				   "passwordCrypt        TEXT NULL, "
				   "name                 TEXT NULL "
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Admins\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Categories( "
				   "id                   INTEGER " + autoincExpr +
				   "name                 TEXT NULL "
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Categories\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Places( "
				   "id                    INTEGER " + autoincExpr +
				   "title                 TEXT NULL, "
				   "address               TEXT NULL, "
				   "id_background         INTEGER NULL, "
				   "backgroundWidth       INTEGER DEFAULT 400, "
				   "backgroundHeight      INTEGER DEFAULT 300"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Places\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS PlaceSchemes( "
				   "id                   INTEGER " + autoincExpr +
				   "seatNumber           TEXT NULL, "
				   "row                  TEXT NULL, "
				   "x                    INTEGER NULL, "
				   "y                    INTEGER NULL, "
				   "id_place             INTEGER NOT NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("PlaceSchemes\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Clients( "
				   "id                   INTEGER " + autoincExpr +
				   "name                 TEXT NULL, "
				   "birthDate            DATE NULL,  "
				   "login                TEXT NULL,  "
				   "passwordHash         TEXT NULL,  "
				   "phone                TEXT NULL  "
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Clients\n");
	}

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
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Actions\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS ActionScheme( "
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NOT NULL, "
				   "state				 INTEGER DEFAULT 0, "
				   "id_priceGroup        INTEGER NULL, "
				   "PRIMARY KEY (id_action, id_placeScheme)"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("ActionScheme\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS ActionPriceGroups( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "name                 TEXT NULL, "
				   "price                INTEGER DEFAULT 0, "
				   "penalty              INTEGER DEFAULT 0, "
				   "color                TEXT NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("ActionPriceGroups\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Markets( "
				   "id                   INTEGER " + autoincExpr +
				   "address              TEXT NULL "
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Markets\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Images( "
				   "id                   INTEGER " + autoincExpr +
				   "image                MEDIUMBLOB NULL, "
				   "dateTime             DATETIME NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Images\n");
	}

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
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Tickets\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS ReturnedTickets( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "id_client            INTEGER NULL, "
				   "identifier           TEXT NOT NULL, "
				   "id_market            INTEGER NULL, "
				   "id_user              INTEGER NULL, "
				   "penalty              INTEGER DEFAULT 0, "
				   "dateTime             DATETIME NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("ReturnedTickets\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Reservations( "
				   "id                   INTEGER " + autoincExpr +
				   "id_action            INTEGER NOT NULL, "
				   "id_placeScheme       INTEGER NULL, "
				   "id_client            INTEGER NULL, "
				   "dateTime             DATETIME NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Reservations\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Statistics( "
				   "id                   INTEGER " + autoincExpr +
				   "type                 INTEGER NOT NULL, "
				   "text                 TEXT NULL, "
				   "data                 BLOB NULL, "
				   "dateTime             DATETIME NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Statistics\n");
	}

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
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("ComplitedActions\n");
	}

	if(!query.exec("CREATE TABLE IF NOT EXISTS Advertisements( "
				   "id                   INTEGER NOT NULL"
				   + tableTypeExpr))
	{
		qDebug(qPrintable(query.lastError().text()));
		tablesError.append("Advertisements\n");
	}

	//removing triggers:

	QList<QString> triggers;
	const QString isExistTriggerExpr = mServer ? "" : "IF EXISTS";
	if(mServer)
	{
		if(query.exec("SHOW TRIGGERS;"))
		{
			while(query.next())
				triggers.append(query.value(0).toString());
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
	else
	{
		triggers << "on_deleteUsers" << "on_deleteActions" << "on_deletePlaces" << "on_insertTickets" << "on_insertReturnedTickets" << "on_insertStatistics" << "on_deleteAdvertisements";
	}
	for(int i = 0 ; i < triggers.count(); i++)
		if(!query.exec("DROP TRIGGER " + isExistTriggerExpr + " `" + triggers[i] + "`;"))
			qDebug(qPrintable(query.lastError().text()));

	//triggers:

	QString triggersError;
	const QString nowExpr = mServer ? "NOW()" : "datetime('now')";

	if(!query.exec("CREATE TRIGGER on_deleteUsers BEFORE DELETE ON Users"
				   " FOR EACH ROW BEGIN"
				   "	UPDATE Tickets SET id_user = NULL WHERE id_user = OLD.id;"
				   " END"
				   ";"))
	{
		qDebug(qPrintable(query.lastError().text()));
		triggersError.append("on_deleteUsers\n");
	}

	if(!query.exec("CREATE TRIGGER on_deleteActions BEFORE DELETE ON Actions"
				   " FOR EACH ROW BEGIN"
				   "	DELETE FROM Tickets WHERE id_action = OLD.id;"
				   "	DELETE FROM ReturnedTickets WHERE id_action = OLD.id;"
				   "	DELETE FROM Reservations WHERE id_action = OLD.id;"
				   "	DELETE FROM ActionScheme WHERE id_action = OLD.id;"
				   "	DELETE FROM ActionPriceGroups WHERE id_action = OLD.id;"
				   "	DELETE FROM Images WHERE id = OLD.id_ticketSubstrate;"
				   " END"
				   ";"))
	{
		qDebug(qPrintable(query.lastError().text()));
		triggersError.append("on_deleteActions\n");
	}

	if(!query.exec("CREATE TRIGGER on_deletePlaces BEFORE DELETE ON Places"
				   " FOR EACH ROW BEGIN"
				   "	DELETE FROM Actions WHERE id_place = OLD.id;"
				   "	DELETE FROM PlaceSchemes WHERE id_place = OLD.id;"
				   "	DELETE FROM Images WHERE id = OLD.id_background;"
				   " END"
				   ";"))
	{
		qDebug(qPrintable(query.lastError().text()));
		triggersError.append("on_deletePlaces\n");
	}

	if(!query.exec("CREATE TRIGGER on_deleteAdvertisements BEFORE DELETE ON Advertisements"
				   " FOR EACH ROW BEGIN"
				   "	DELETE FROM Images WHERE id = OLD.id;"
				   " END"
				   ";"))
	{
		qDebug(qPrintable(query.lastError().text()));
		triggersError.append("on_deleteAdvertisements\n");
	}

	if(mServer)
	{
		if(!query.exec("CREATE TRIGGER on_insertTickets BEFORE INSERT ON Tickets"
					   " FOR EACH ROW BEGIN"
					   "	SET NEW.dateTime = " + nowExpr + ";"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertTickets\n");
		}

		if(!query.exec("CREATE TRIGGER on_insertReturnedTickets BEFORE INSERT ON ReturnedTickets"
					   " FOR EACH ROW BEGIN"
					   "	SET NEW.dateTime = " + nowExpr + ";"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertReturnedTickets\n");
		}

		if(!query.exec("CREATE TRIGGER on_insertStatistics BEFORE INSERT ON Statistics"
					   " FOR EACH ROW BEGIN"
					   "	SET NEW.dateTime = " + nowExpr + ";"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertStatistics\n");
		}
	}
	else
	{
		if(!query.exec("CREATE TRIGGER on_insertTickets AFTER INSERT ON Tickets"
					   " FOR EACH ROW BEGIN"
					   "	UPDATE Tickets SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertTickets\n");
		}

		if(!query.exec("CREATE TRIGGER on_insertReturnedTickets AFTER INSERT ON ReturnedTickets"
					   " FOR EACH ROW BEGIN"
					   "	UPDATE ReturnedTickets SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertReturnedTickets\n");
		}

		if(!query.exec("CREATE TRIGGER on_insertStatistics AFTER INSERT ON Statistics"
					   " FOR EACH ROW BEGIN"
					   "	UPDATE Statistics SET dateTime = " + nowExpr + " WHERE id = NEW.id;"
					   " END"
					   ";"))
		{
			qDebug(qPrintable(query.lastError().text()));
			triggersError.append("on_insertStatistics\n");
		}
	}

	showTables();

	QString error;
	if(tablesError.isEmpty() == false)
		error = tr("Не удалось создать следующие таблицы:\n%1\n\n").arg(tablesError);
	if(triggersError.isEmpty() == false)
		error.append(tr("Не удалось создать следующие триггеры:\n%1").arg(triggersError));
	if(error.isEmpty() == false)
		QMessageBox::warning(this, tr("Внимание"), error);
	else
		QMessageBox::information(this, tr("Успех"), tr("Все недостающие таблицы успешно созданы и все триггеры обновлены."));
}
