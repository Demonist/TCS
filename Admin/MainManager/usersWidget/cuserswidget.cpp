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

	if(query.exec("SELECT Users.id, login, passwordCrypt, name, address FROM Users, Markets WHERE marketsID = Markets.ID;"))
	{
		ui->twUsers->clear();

		uint count = 0;
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(LOGIN, query.value(1).toString());
				item->setText(PASSWORD, query.value(2).toString());
				item->setText(NAME, query.value(3).toString());
				item->setText(MARKET, query.value(4).toString());
				ui->twUsers->addTopLevelItem(item);
				count++;
			}
			////TODO: Добавить обработчик ошибок.
		}
		ui->gbxUsers->setTitle(tr("Пользователи системы: %1 шт").arg(count));
	}
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
