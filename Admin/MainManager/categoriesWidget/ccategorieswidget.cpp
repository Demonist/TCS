#include "ccategorieswidget.h"
#include "ui_ccategorieswidget.h"


#define ID 0
#define NAME 1

CCategoriesWidget::CCategoriesWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CCategoriesWidget)
{
	ui->setupUi(this);
	ui->twCategories->hideColumn(ID);
	ui->twCategories->setColumnWidth(ID, 0);
	ui->twCategories->setColumnWidth(NAME, 200);
}

CCategoriesWidget::~CCategoriesWidget()
{
	delete ui;
}

void CCategoriesWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT id, name FROM Categories;"))
	{
		ui->twCategories->clear();
		uint count = 0;
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(NAME, query.value(1).toString());

				ui->twCategories->addTopLevelItem(item);
				count++;
			}
		}
		ui->groupBox->setTitle(tr("Категории: %1 шт").arg(count));
	}
}


void CCategoriesWidget::on_tbnCategoriesAdd_clicked()
{
	CCategoryDialog categoriesDialog(mConnectionName, this);
	connect(&categoriesDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
	categoriesDialog.exec();
}

void CCategoriesWidget::on_tbnCategoriesEdit_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twCategories->currentItem();
	if(selectedItem)
	{
		CCategoryDialog categoriesDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
		connect(&categoriesDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
		categoriesDialog.exec();
	}
}

void CCategoriesWidget::on_tbnCategoriesDelete_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twCategories->currentItem();
	if(selectedItem)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную категорию?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Categories WHERE id = :id");
			query.bindValue(":id", selectedItem->text(ID));
			query.exec();
			updateData();
		}
	}
}
