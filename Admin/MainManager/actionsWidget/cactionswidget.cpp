#include "cactionswidget.h"
#include "ui_cactionswidget.h"

#define ID 0
#define TITLE 1
#define PLACE 2
#define DATETIME 3
#define STATE 4
#define CATEGORY 5
#define PERFORMER 6

CActionsWidget::CActionsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
    ui(new Ui::CActionsWidget)
{
    ui->setupUi(this);
	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);

	mCategoriesCount = 0;
	mPlacesCount = 0;
}

CActionsWidget::~CActionsWidget()
{
    delete ui;
}

void CActionsWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(query.exec("SELECT COUNT(id) FROM Categories;") && query.first())
		mCategoriesCount = query.value(0).toInt();
	if(query.exec("SELECT COUNT(id) FROM Places;") && query.first())
		mPlacesCount = query.value(0).toInt();

	if(query.exec("SELECT Actions.id, Actions.title, Places.title, Places.address, Actions.dateTime, Actions.state, Categories.name, Actions.performer FROM Actions, Places, Categories WHERE Actions.id_place = Places.id AND Actions.id_category = Categories.id;"))
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
							  .arg(dateTime.toString("dd.MM.yyyy hh:mm"))
							  .arg(QDate::longDayName(dateTime.date().dayOfWeek()))
							  );
				item->setText(STATE, Global::actionStateToText(query.value(5).toInt()));
				item->setText(CATEGORY, query.value(6).toString());
				item->setText(PERFORMER, query.value(7).toString());

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
	QString error;
	if(mCategoriesCount == 0)
		error += tr("Невозможно добавить мероприятие т.к. нет ни одной категории.\nСначала добавте хотя бы одну категорию.\n\n");
	if(mPlacesCount == 0)
		error += tr("Невозможно добавить мероприятие т.к. нет ни одной концертной площадки.\nСначала добавте хотя бы одну концертную площадку.\n\n");

	if(error.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Внимание"), error);
		return;
	}

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
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		int solded = 0;
		query.prepare("SELECT COUNT(id) FROM Tickets WHERE id_action = :id;");
		query.bindValue(":id", selectedItem->text(ID).toInt());
		if(query.exec() && query.first())
			solded = query.value(0).toInt();
		int reserved = 0;
		query.prepare("SELECT COUNT(id) FROM Reservations WHERE id_action = :id;");
		query.bindValue(":id", selectedItem->text(ID));
		if(query.exec() && query.first())
			reserved = query.value(0).toInt();
		if(solded + reserved > 0)
		{
			QMessageBox::warning(this, tr("Внимание"), tr("Невозможно удалить данное мероприятие так как уже продано %1 и забронировано %2 билетов.\nУдаление мероприятия невозможно. Но Вы можете изменить его состояние на '%3' - это равносильно удалению.").arg(solded).arg(reserved).arg(Global::actionStateToText(Global::ActionComplited)));
			return;
		}
		else
			if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранное мероприятие?"), QMessageBox::Yes, QMessageBox::No))
			{
				query.prepare("DELETE FROM Actions WHERE id = :id");
				query.bindValue(":id", selectedItem->text(ID));
				query.exec();
				updateData();
			}
	}
}
