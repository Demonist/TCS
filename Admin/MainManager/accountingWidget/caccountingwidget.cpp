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

#define COMPLITED_ID 0
#define COMPLITED_TITLE 1
#define COMPLITED_PERFORMER 2
#define COMPLITED_PLACE 3
#define COMPLITED_CATEGORY 4
#define COMPLITED_SOLDED 5
#define COMPLITED_RETURNED 6
#define COMPLITED_INCOME 7
#define COMPLITED_PENALTY 8

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

	ui->twComplitedActions->hideColumn(0);
	ui->twComplitedActions->setColumnWidth(0, 0);
}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	// Возвращенные билеты:
	static CCacheChecker cacheCheckerReturnedTicket("ReturnedTickets, Markets, Users, Actions", mConnectionName);
	if(cacheCheckerReturnedTicket.isNeedUpdate())
	{
		cacheCheckerReturnedTicket.setUpdated();

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

	//Продажи мероприятий:
	static CCacheChecker cacheCheckerActions("Actions, Places, Categories Tickets Markets Users ReturnedTickets", mConnectionName);
	if(cacheCheckerActions.isNeedUpdate())
	{
		cacheCheckerActions.setUpdated();

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

	//Завершенные мероприятия:
	static CCacheChecker cacheCheckerComplitedActions("ComplitedActions", mConnectionName);
	if(cacheCheckerComplitedActions.isNeedUpdate())
	{
		cacheCheckerComplitedActions.setUpdated();

		if(query.exec("SELECT id, title, performer, place, category, ticketsSolded, ticketsReturned, income, penalties, data FROM ComplitedActions ORDER BY id DESC;"))
		{
			ui->twComplitedActions->clear();
			mComplitedStatistics.clear();

			QTreeWidgetItem *item;
			CComplitedAction action;
			while(query.next())
			{
				item = new QTreeWidgetItem();
				if(item)
				{
					item->setText(COMPLITED_ID, query.value(0).toString());
					item->setText(COMPLITED_TITLE, query.value(1).toString());
					item->setText(COMPLITED_PERFORMER, query.value(2).toString());
					item->setText(COMPLITED_PLACE, query.value(3).toString());
					item->setText(COMPLITED_CATEGORY, query.value(4).toString());
					item->setText(COMPLITED_SOLDED, query.value(5).toString());
					item->setText(COMPLITED_RETURNED, query.value(6).toString());
					item->setText(COMPLITED_INCOME, query.value(7).toString());
					item->setText(COMPLITED_PENALTY, query.value(8).toString());

					ui->twComplitedActions->addTopLevelItem(item);

					action.soldedFromByteArray(query.value(9).toByteArray());
					mComplitedStatistics.append(qMakePair(action.soldedByMarkets, action.soldedBySellers));
				}
			}
		}
		else
			qDebug(qPrintable(query.lastError().text()));
	}
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

void CAccountingWidget::on_twComplitedActions_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	int index = ui->twComplitedActions->indexOfTopLevelItem(current);
	if(index >= 0 && index < mComplitedStatistics.size())
	{
		ui->twComplitedMarkets->clear();
		ui->twComplitedSellers->clear();

		const QHash<QString, int> &markets = mComplitedStatistics[index].first;
		const QHash<QString, int> &selers = mComplitedStatistics[index].second;

		QTreeWidgetItem *item;
		QList<QString> keys = markets.keys();
		for(int i = 0; i < keys.count(); i++)
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(0, keys[i]);
				item->setText(1, QString::number(markets[keys[i]]));
				ui->twComplitedMarkets->addTopLevelItem(item);
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
				ui->twComplitedSellers->addTopLevelItem(item);
			}
		}
	}
	else
		qDebug("Complited statistic alarme!");
}

void CAccountingWidget::on_pbnExport_clicked()
{
	if(ui->twComplitedActions->topLevelItemCount() == 0)
	{
		QMessageBox::warning(this, tr("Внимание"), tr("Нет данных для экспорта."));
		return;
	}

	static QString exportPath;
	if(exportPath.isEmpty())
		exportPath = Global::currentPath() + tr("/export/Отчеты");
	QDir().mkpath(exportPath);

	QString fileName = QFileDialog::getSaveFileName(this, tr("Укажите имя файла для сохранения"), exportPath + QDateTime::currentDateTime().toString("/yyyy_MM_dd hh-mm.csv"), tr("Файлы CSV (*.csv)"));
	if(fileName.isEmpty() == false)
	{
		QFileInfo fileInfo(fileName);
		if(fileInfo.completeSuffix() != "csv")
			fileName = fileInfo.absolutePath() + '/' + fileInfo.baseName() + ".csv";
		QFile file(fileName);
		if(file.open(QFile::WriteOnly))
		{
			file.write(tr("\"Мероприятие\",\"Исполнитель\",\"Площадка\",\"Категория\",\"Продано билетов\",\"Возвращено билетов\",\"Общая прибыль\",\"Прибыль с неустоек\"\n").toLocal8Bit());
			for(int i = 0; i < ui->twComplitedActions->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *item = ui->twComplitedActions->topLevelItem(i);
				file.write(
							tr("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",\"%6\",\"%7\",\"%8\"\n")
							.arg(item->text(COMPLITED_TITLE))
							.arg(item->text(COMPLITED_PERFORMER))
							.arg(item->text(COMPLITED_PLACE))
							.arg(item->text(COMPLITED_CATEGORY))
							.arg(item->text(COMPLITED_SOLDED))
							.arg(item->text(COMPLITED_RETURNED))
							.arg(item->text(COMPLITED_INCOME))
							.arg(item->text(COMPLITED_PENALTY))
							.toLocal8Bit()
							);
			}
			file.write("\n");

			file.write(tr("\"Продажи торговых точек\"\n\"Торговая площадка\",\"Продано\"\n").toLocal8Bit());
			for(int i = 0; i < ui->twComplitedMarkets->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *item = ui->twComplitedMarkets->topLevelItem(i);
				file.write(
							tr("\"%1\",\"%2\"\n")
							.arg(item->text(0))
							.arg(item->text(1))
							.toLocal8Bit()
							);
			}
			file.write("\n");

			file.write(tr("\"Продажи кассиров\"\n\"Кассир\",\"Продано\"\n").toLocal8Bit());
			for(int i = 0; i < ui->twComplitedSellers->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *item = ui->twComplitedSellers->topLevelItem(i);
				file.write(
							tr("\"%1\",\"%2\"\n")
							.arg(item->text(0))
							.arg(item->text(1))
							.toLocal8Bit()
							);
			}
			file.write("\n");

			file.close();
		}
	}
}
