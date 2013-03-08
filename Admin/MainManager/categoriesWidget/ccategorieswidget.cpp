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
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("SELECT COUNT(id) FROM Actions WHERE Actions.id_category = :catId;");
		query.bindValue(":catId", selectedItem->text(ID));
		if(query.exec() && query.first())
		{
			if(query.value(0).toInt() > 0)
			{
				QMessageBox::warning(this, tr("Внимание"), tr("Невозможно удалить данную категорию так как к ней привязано %1 мероприятий.\nИзмените категорию у данных мероприятий или удалите их или измените их статус на '%2'.").arg(query.value(0).toString()).arg(Global::actionStateToText(Global::ActionComplited)));
				return;
			}
			else
				if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную категорию?"), QMessageBox::Yes, QMessageBox::No))
				{
					query.prepare("DELETE FROM Categories WHERE id = :id");
					query.bindValue(":id", selectedItem->text(ID));
					query.exec();
					updateData();
				}
		}
	}
}
