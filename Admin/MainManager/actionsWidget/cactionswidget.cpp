#include "cactionswidget.h"
#include "ui_cactionswidget.h"

#define ID 0
#define TITLE 1
#define PLACE 2
#define DATETIME 3
#define STATE 4
#define CATEGORY 5

CActionsWidget::CActionsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
    ui(new Ui::CActionsWidget)
{
    ui->setupUi(this);
	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);
}

CActionsWidget::~CActionsWidget()
{
    delete ui;
}

void CActionsWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(query.exec("SELECT Actions.id, Actions.title, Places.title, Places.address, Actions.dateTime, Actions.state, Categories.name FROM Actions, Places, Categories WHERE Actions.id_place == Places.id AND Actions.id_category == Categories.id;"))
	{
		ui->twActions->clear();

		uint count = 0;
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(TITLE, query.value(1).toString());
				item->setText(PLACE, query.value(2).toString() + " (" + query.value(3).toString() + ')');
				QDateTime dateTime = query.value(4).toDateTime();
				item->setText(DATETIME, tr("%1 (%2)")
							  .arg(dateTime.toString("dd.MM.yyyy"))
							  .arg(QDate::longDayName(dateTime.date().dayOfWeek()))
							  );
				item->setText(STATE, Global::actionStateToText(query.value(5).toInt()));
				item->setText(CATEGORY, query.value(6).toString());

				ui->twActions->addTopLevelItem(item);
				count++;
			}
			////TODO: Добавить обработчик ошибок.
		}
		ui->gbxActions->setTitle(tr("Мероприятия: %1 шт").arg(count));
	}
	else qDebug(qPrintable(query.lastError().text()));
}

void CActionsWidget::on_tbnAdd_clicked()
{
	CActionDialog actionDialog(mConnectionName, this);
	connect(&actionDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	actionDialog.exec();
}

void CActionsWidget::on_tbnEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twActions->currentItem();
	if(selectedItem)
	{
		CActionDialog actionDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&actionDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		actionDialog.exec();
	}
}

void CActionsWidget::on_tbnDel_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twActions->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранное мероприятие?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Actions WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();

			////TODO: Сделать каскадное удаление.

			updateData();
		}
	}
}