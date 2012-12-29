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


void CPlacesWidget::on_pbPlacesAdd_clicked()
{
	CPlaceDialog placeDialog(mConnectionName, this);
	connect(&placeDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	placeDialog.exec();
}

void CPlacesWidget::on_pbPlacesEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twPlaces->currentItem();
	if(selectedItem)
	{
		CPlaceDialog placeDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&placeDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		placeDialog.exec();
	}
}

void CPlacesWidget::on_pbPlacesDelete_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twPlaces->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную площадку?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Places WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();
			updateData();
		}
	}
}
