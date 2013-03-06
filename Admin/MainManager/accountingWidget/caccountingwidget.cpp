#include "caccountingwidget.h"
#include "ui_caccountingwidget.h"

#define RETURNED_MARKET 0
#define RETURNED_SELLER 1
#define RETURNED_ACTION 2

CAccountingWidget::CAccountingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CAccountingWidget)
{
    ui->setupUi(this);
}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT data FROM Statistics WHERE type = :type;");
	query.bindValue(":type", CStatistics::TypeTicketReturned);
	if(query.exec())
	{
		QTreeWidgetItem *item;
		while(query.next())
		{
			CStatisticTicketReturnedType *type = (CStatisticTicketReturnedType*)CStatistics::fromByteArray(query.value(0).toByteArray());
			if(type && type->isValid())
			{
				item = new QTreeWidgetItem();
				if(item)
				{
					QSqlQuery query2(QSqlDatabase::database(mConnectionName));
					query2.prepare("SELECT Markets.address, Users.name, Actions.title FROM Markets, Users, Actions WHERE Markets.id = :marketId AND Users.id = :userId AND Actions.id = :actId;");
					query2.bindValue(":marketId", type->marketId);
					query2.bindValue(":userId", type->sellerId);
					query2.bindValue(":actId", type->actionId);
					if(query2.exec() && query2.first())
					{
						item->setText(RETURNED_MARKET, query2.value(0).toString());
						item->setText(RETURNED_SELLER, query2.value(1).toString());
						item->setText(RETURNED_ACTION, query2.value(2).toString());
						ui->twReturnedTickets->addTopLevelItem(item);
					}
					else
						qDebug(qPrintable(query2.lastError().text()));
				}
			}
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}

