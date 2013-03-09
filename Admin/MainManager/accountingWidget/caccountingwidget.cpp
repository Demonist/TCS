#include "caccountingwidget.h"
#include "ui_caccountingwidget.h"

#define RETURNED_ID 0
#define RETURNED_MARKET 1
#define RETURNED_SELLER 2
#define RETURNED_ACTION 3
#define RETURNED_DATETIME 4

#define ACTIONS_ID 0
#define ACTIONS_TITLE 1
#define ACTIONS_STATE 2
#define ACTIONS_PERFORMER 3
#define ACTIONS_PLACE 4
#define ACTIONS_CATEGORY 5
#define ACTIONS_SOLDED 6
#define ACTIONS_RETURNED 7
#define ACTIONS_INCOME 8
#define ACTIONS_PENALTY 9

CAccountingWidget::CAccountingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CAccountingWidget)
{
    ui->setupUi(this);
	ui->tabWidget->setCurrentIndex(0);

	ui->twReturnedTickets->hideColumn(0);
	ui->twReturnedTickets->setColumnWidth(0, 0);

	ui->twActions->hideColumn(0);
	ui->twActions->setColumnWidth(0, 0);
}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	// Возвращенные билеты:
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

	//Продажи мероприятий:
	if(query.exec("SELECT id, state FROM Actions"))
	{
		ui->twActions->clear();
		mActionStatistics.clear();

		QTreeWidgetItem *item;
		CComplitedAction action;
		while(query.next())
		{
			if(action.makeFromAction(query.value(0).toInt(), mConnectionName))
			{
				item = new QTreeWidgetItem();
				if(item)
				{
					item->setText(ACTIONS_ID, query.value(0).toString());
					item->setText(ACTIONS_TITLE, action.title);
					item->setText(ACTIONS_STATE, Global::actionStateToText(query.value(1).toInt()));
					item->setText(ACTIONS_PERFORMER, action.performer);
					item->setText(ACTIONS_PLACE, action.place);
					item->setText(ACTIONS_CATEGORY, action.category);
					item->setText(ACTIONS_SOLDED, QString::number(action.totalTicketsSolded));
					item->setText(ACTIONS_RETURNED, QString::number(action.ticketsReturned));
					item->setText(ACTIONS_INCOME, QString::number(action.totalSumm));
					item->setText(ACTIONS_PENALTY, QString::number(action.penaltySumm));

					ui->twActions->addTopLevelItem(item);
					mActionStatistics.append(qMakePair(action.soldedByMarkets, action.soldedBySellers));
				}
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

void CAccountingWidget::on_twActions_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	int index = ui->twActions->indexOfTopLevelItem(current);
	if(index >= 0 && index < mActionStatistics.size())
	{
		ui->twMarkets->clear();
		ui->twSellers->clear();

		const QHash<QString, int> &markets = mActionStatistics[index].first;
		const QHash<QString, int> &selers = mActionStatistics[index].second;
		QTreeWidgetItem *item;
		QList<QString> keys = markets.keys();
		for(int i = 0; i < keys.count(); i++)
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(0, keys[i]);
				item->setText(1, QString::number(markets[keys[i]]));
				ui->twMarkets->addTopLevelItem(item);
			}
		}
		keys = selers.keys();
		for(int i = 0; i < keys.count(); i++)
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(0, keys[i]);
				item->setText(1, QString::number(selers[keys[i]]));
				ui->twSellers->addTopLevelItem(item);
			}
		}
	}
	else
		qDebug("Actions statistic alarme!");
}
