#include "cclientswidget.h"
#include "ui_cclientswidget.h"

#define ID 0
#define NAME 1
#define BIRTHDATE 2
#define LOGIN 3
#define PASSWORD 4
#define PHONE 5

CClientsWidget::CClientsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CClientsWidget)
{
	ui->setupUi(this);
	ui->twClients->hideColumn(ID);
	ui->twClients->setColumnWidth(ID, 0);
	ui->twClients->setColumnWidth(NAME, 200);
	ui->twClients->setColumnWidth(BIRTHDATE, 200);
	ui->twClients->setColumnWidth(LOGIN, 200);
	ui->twClients->setColumnWidth(PASSWORD, 200);
    ui->twClients->setColumnWidth(PHONE, 200);
}

CClientsWidget::~CClientsWidget()
{
	delete ui;
}

void CClientsWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT id, name, birthDate, login, passwordHash, phone FROM Clients;"))
	{
		ui->twClients->clear();
		uint count = 0;
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(NAME, query.value(1).toString());
				item->setText(BIRTHDATE, query.value(2).toString());
				item->setText(LOGIN, query.value(3).toString());
				item->setText(PASSWORD, Global::decrypt(query.value(4).toByteArray()));
                item->setText(PHONE, query.value(5).toString());

				ui->twClients->addTopLevelItem(item);
				count++;
			}
		}
		ui->groupBox->setTitle(tr("Клиенты: %1 шт").arg(count));
	}
}

void CClientsWidget::on_tbnClientsAdd_clicked()
{
	CClientDialog clientDialog(mConnectionName, this);
	connect(&clientDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	clientDialog.exec();
}

void CClientsWidget::on_tbnClientsEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twClients->currentItem();
	if(selectedItem)
	{
		CClientDialog clientDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&clientDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		clientDialog.exec();
	}
}

void CClientsWidget::on_tbnClientsDelete_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twClients->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранного клиента?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Clients WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();
			updateData();
		}
	}
}
