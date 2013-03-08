#include "caccountingwidget.h"
#include "ui_caccountingwidget.h"

#define RETURNED_ID 0
#define RETURNED_MARKET 1
#define RETURNED_SELLER 2
#define RETURNED_ACTION 3
#define RETURNED_DATETIME 4

CAccountingWidget::CAccountingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CAccountingWidget)
{
    ui->setupUi(this);
	ui->twReturnedTickets->hideColumn(0);
	ui->twReturnedTickets->setColumnWidth(0, 0);
}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT DISTINCT ReturnedTickets.id_action, Actions.title FROM ReturnedTickets, Actions WHERE ReturnedTickets.id_action = Actions.id;"))
	{
		ui->cbxActions->clear();
		ui->cbxActions->addItem(tr("Все"), 0);

		while(query.next())
			ui->cbxActions->addItem(query.value(1).toString(), query.value(0));
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	if(query.exec("SELECT ReturnedTickets.id, Markets.address, Users.name, Actions.title, ReturnedTickets.id_action, ReturnedTickets.dateTime FROM ReturnedTickets, Markets, Users, Actions WHERE Markets.id = ReturnedTickets.id_market AND Users.id = ReturnedTickets.id_user AND Actions.id = ReturnedTickets.id_action"))
	{
		ui->twReturnedTickets->clear();

		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(RETURNED_ID, query.value(0).toString());
				item->setText(RETURNED_MARKET, query.value(1).toString());
				item->setText(RETURNED_SELLER, query.value(2).toString());
				item->setText(RETURNED_ACTION, query.value(3).toString());
				item->setData(RETURNED_ACTION, Qt::UserRole, query.value(4));
				item->setText(RETURNED_DATETIME, query.value(5).toDateTime().toString("dd.MM.yyyy hh:mm"));

				ui->twReturnedTickets->addTopLevelItem(item);
			}
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}


void CAccountingWidget::on_cbxActions_currentIndexChanged(int index)
{
	for(int i = 0; i < ui->twReturnedTickets->topLevelItemCount(); i++)
	{
		if(index == 0 || ui->cbxActions->itemData(index) == ui->twReturnedTickets->topLevelItem(i)->data(RETURNED_ACTION, Qt::UserRole))
			ui->twReturnedTickets->topLevelItem(i)->setHidden(false);
		else
			ui->twReturnedTickets->topLevelItem(i)->setHidden(true);
	}
}
