#include "cplaceswidget.h"
#include "ui_cplaceswidget.h"

#define ID 0
#define TITLE 1
#define ADDRESS 2

CPlacesWidget::CPlacesWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CPlacesWidget)
{
	ui->setupUi(this);
	ui->twPlaces->hideColumn(ID);
	ui->twPlaces->setColumnWidth(ID, 0);
	ui->twPlaces->setColumnWidth(TITLE, 200);
	ui->twPlaces->setColumnWidth(ADDRESS, 200);
}

CPlacesWidget::~CPlacesWidget()
{
	delete ui;
}

void CPlacesWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT id, title, address FROM Places;"))
	{
		ui->twPlaces->clear();
		uint count = 0;
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(TITLE, query.value(1).toString());
				item->setText(ADDRESS, query.value(2).toString());
				ui->twPlaces->addTopLevelItem(item);
				count++;
			}
		}
		ui->groupBox->setTitle(tr("Концертные площадки: %1 шт").arg(count));
	}
}


void CPlacesWidget::on_tbnPlacesAdd_clicked()
{
	CPlaceDialog placeDialog(mConnectionName, this);
	connect(&placeDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	placeDialog.exec();
}

void CPlacesWidget::on_tbnPlacesEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twPlaces->currentItem();
	if(selectedItem)
	{
		CPlaceDialog placeDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&placeDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		placeDialog.exec();
	}
}

void CPlacesWidget::on_tbnPlacesDelete_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twPlaces->currentItem();
	if(selectedItem)
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT COUNT(id) FROM Actions WHERE id_place = :id;");
		query.bindValue(":id", selectedItem->text(ID));
		if(query.exec() && query.first())
		{
			if(query.value(0).toInt() > 0)
			{
				QMessageBox::warning(this, tr("Внимание"), tr("Невозможно удалить данную площадку так как к ней привязано %1 мероприятий.\nУдалите привязанные мероприятия или измените их статус на '%2'.").arg(query.value(0).toString()).arg(Global::actionStateToText(Global::ActionComplited)));
				return;
			}
			else
				if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную площадку?"), QMessageBox::Yes, QMessageBox::No))
				{
					query.prepare("DELETE FROM Places WHERE id = :id");
					query.bindValue(":id", selectedItem->text(ID));
					query.exec();
					updateData();
				}
		}
	}
}
