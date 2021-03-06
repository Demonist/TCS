#include "cuserswidget.h"
#include "ui_cuserswidget.h"

#define ID 0
#define LOGIN 1
#define PASSWORD 2
#define NAME 3
#define MARKET 4

CUsersWidget::CUsersWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CUsersWidget)
{
	ui->setupUi(this);
	ui->twUsers->hideColumn(ID);
	ui->twUsers->setColumnWidth(ID, 0);
	ui->twUsers->setColumnWidth(LOGIN, 200);
	ui->twUsers->setColumnWidth(PASSWORD, 200);
	ui->twUsers->setColumnWidth(NAME, 200);
	ui->twUsers->setColumnWidth(MARKET, 200);
	ui->twAdmins->hideColumn(ID);
	ui->twAdmins->setColumnWidth(ID, 0);
	ui->twAdmins->setColumnWidth(LOGIN, 200);
	ui->twAdmins->setColumnWidth(PASSWORD, 200);
	ui->twAdmins->setColumnWidth(NAME, 200);
	ui->twAdmins->setColumnWidth(MARKET, 200);
	ui->tabWidget->setCurrentIndex(0);

	mMarketsCount = 0;
}

CUsersWidget::~CUsersWidget()
{
	delete ui;
}

void CUsersWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(query.exec("SELECT COUNT(id) FROM Markets;") && query.first())
		mMarketsCount = query.value(0).toInt();

	uint count = 0;
	QTreeWidgetItem *item;

	if(query.exec("SELECT Users.id, Users.login, Users.passwordCrypt, Users.name, Markets.address FROM Users, Markets WHERE Users.id_market = Markets.id;"))
	{
		ui->twUsers->clear();

		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(LOGIN, query.value(1).toString());
				item->setText(PASSWORD, Global::decrypt(query.value(2).toByteArray()));
				item->setText(NAME, query.value(3).toString());
				item->setText(MARKET, query.value(4).toString());
				ui->twUsers->addTopLevelItem(item);
				count++;
			}
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	if(query.exec("SELECT id, login, passwordCrypt, name FROM Admins;"))
	{
		ui->twAdmins->clear();

		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(LOGIN, query.value(1).toString());
				item->setText(PASSWORD, Global::decrypt(query.value(2).toByteArray()));
				item->setText(NAME, query.value(3).toString());
				ui->twAdmins->addTopLevelItem(item);
				count++;
			}
		}
	}

	ui->gbxUsers->setTitle(tr("Пользователи системы: %1 шт").arg(count));
}

void CUsersWidget::on_tbnAdd_clicked()
{
	if(mMarketsCount == 0)
	{
		QMessageBox::warning(this, tr("Внимание"), tr("Невозможно добавить пользователя т.к. нет ни одной торговой точки.\nСначала добавте хотя бы одну торговую точку."));
		return;
	}

	CUserDialog userDialog(mConnectionName, this);
	connect(&userDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	userDialog.exec();
}

void CUsersWidget::on_tbnEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twUsers->currentItem();
	if(selectedItem)
	{
		CUserDialog userDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&userDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		userDialog.exec();
	}
}

void CUsersWidget::on_tbnDel_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twUsers->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранного пользователя?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Users WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();

			updateData();
		}
	}
}

void CUsersWidget::on_tbnAdminAdd_clicked()
{
	CUserAdminDialog userDialog(mConnectionName, this);
	connect(&userDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	userDialog.exec();
}

void CUsersWidget::on_tbnAdminEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twAdmins->currentItem();
	if(selectedItem)
	{
		CUserAdminDialog userDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&userDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		userDialog.exec();
	}
}

void CUsersWidget::on_tbnAdminDel_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twAdmins->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранного пользователя?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Admins WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();

			updateData();
		}
	}
}
