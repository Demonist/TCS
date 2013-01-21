#include "cclientactionswidget.h"
#include "ui_cclientactionswidget.h"

#define ID 0
#define TITLE 1
#define PERFORMER 2
#define DATETIME 3
#define PLACE 4
#define CATEGORY 5

CClientActionsWidget::CClientActionsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CClientActionsWidget)
{
	ui->setupUi(this);

	mCanUpdateFilter = true;

	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);

	connect(ui->cbxCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->cbxPerformer, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->cbxPlace, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilter()));
	connect(ui->leDate, SIGNAL(textChanged(QString)), this, SLOT(updateFilter()));
}

CClientActionsWidget::~CClientActionsWidget()
{
	delete ui;
}

void CClientActionsWidget::updateFilter()
{
	if(mCanUpdateFilter)
	{
		QVariant category = ui->cbxCategory->itemData(ui->cbxCategory->currentIndex());
		QVariant place = ui->cbxPlace->itemData(ui->cbxPlace->currentIndex());
		QString performer;
		QString date = ui->leDate->text();

		bool categoryFilter = category.toInt() != 0;
		bool placeFilter = place.toInt() != 0;
		bool performerFilter;
		bool dateFilter;

		if(ui->cbxPerformer->currentIndex())
		{
			performer = ui->cbxPerformer->currentText();
			performerFilter = true;
		}
		else
			performerFilter = false;

		dateFilter = date.isEmpty() == false;

		bool hide;
		QTreeWidgetItem *item;
		for(int i = 0; i < ui->twActions->topLevelItemCount(); i++)
		{
			hide = false;
			item = ui->twActions->topLevelItem(i);

			if(categoryFilter && category != item->data(CATEGORY, Qt::UserRole))
				hide = true;
			else if(placeFilter && place != item->data(PLACE, Qt::UserRole))
				hide = true;
			else if(performerFilter && performer != item->text(PERFORMER))
				hide = true;
			else if(dateFilter && date != item->data(DATETIME, Qt::UserRole))
				hide = true;

			item->setHidden(hide);
		}
	}
}

void CClientActionsWidget::on_tbnCategoryClear_clicked()
{
	ui->cbxCategory->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnPerformerCLear_clicked()
{
	ui->cbxPerformer->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnPlaceClear_clicked()
{
	ui->cbxPlace->setCurrentIndex(0);
}

void CClientActionsWidget::on_tbnDate_clicked()
{
	CDateDialog dateDialog(this);
	if(QDialog::Accepted == dateDialog.exec())
		ui->leDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
}

void CClientActionsWidget::on_tbnDateClear_clicked()
{
	ui->leDate->clear();
}

void CClientActionsWidget::on_tbnFilterClear_clicked()
{
	mCanUpdateFilter = false;
	ui->cbxCategory->setCurrentIndex(0);
	ui->cbxPerformer->setCurrentIndex(0);
	ui->cbxPlace->setCurrentIndex(0);
	ui->leDate->clear();
	mCanUpdateFilter = true;
	updateFilter();
}

void CClientActionsWidget::on_pbnUpdate_clicked()
{
	updateData();
}

//public slots:

void CClientActionsWidget::updateData()
{
	mCanUpdateFilter = false;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	ui->cbxCategory->clear();
	ui->cbxCategory->addItem(tr("Все"), 0);
	if(query.exec("SELECT name, id FROM Categories;"))
		while(query.next())
			ui->cbxCategory->addItem(query.value(0).toString(), query.value(1).toInt());

	ui->cbxPerformer->clear();
	ui->cbxPerformer->addItem(tr("Все"), 0);
	query.prepare("SELECT DISTINCT performer FROM Actions WHERE state = :state OR state = :state2;");
	query.bindValue(":state", Global::ActionActive);
	query.bindValue(":state2", Global::ActionMoved);
	if(query.exec())
		while(query.next())
			ui->cbxPerformer->addItem(query.value(0).toString());

	ui->cbxPlace->clear();
	ui->cbxPlace->addItem(tr("Все"), 0);
	if(query.exec("SELECT title, id FROM Places;"))
		while(query.next())
			ui->cbxPlace->addItem(query.value(0).toString(), query.value(1).toInt());

	ui->leDate->clear();

	query.prepare("SELECT Actions.id, Actions.title, Actions.performer, Actions.dateTime, Places.title, Places.address, Places.id, Categories.name, Categories.id FROM Actions, Places, Categories WHERE Actions.id_place = Places.id AND Actions.id_category = Categories.id AND (Actions.state = :state OR Actions.state = :state2);");
	query.bindValue(":state", Global::ActionActive);
	query.bindValue(":state2", Global::ActionMoved);
	if(query.exec())
	{
		ui->twActions->clear();

		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(TITLE, query.value(1).toString());
				item->setText(PERFORMER, query.value(2).toString());
				QDateTime dateTime = query.value(3).toDateTime();
				item->setText(DATETIME, tr("%1 (%2)")
							  .arg(dateTime.toString("dd.MM.yyyy hh:mm"))
							  .arg(QDate::longDayName(dateTime.date().dayOfWeek()))
							  );
				item->setData(DATETIME, Qt::UserRole, dateTime.date().toString("dd.MM.yyyy"));
				item->setText(PLACE, query.value(4).toString() + " (" + query.value(5).toString() + ')');
				item->setData(PLACE, Qt::UserRole, query.value(6));
				item->setText(CATEGORY, query.value(7).toString());
				item->setData(CATEGORY, Qt::UserRole, query.value(8));

				ui->twActions->addTopLevelItem(item);
			}
			////TODO: Добавить обработчик ошибок.
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	mCanUpdateFilter = true;
}
