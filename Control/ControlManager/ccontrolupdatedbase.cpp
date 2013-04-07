#include "ccontrolupdatedbase.h"
#include "ui_ccontrolupdatedbase.h"

#define CATEGORY 0
#define ACTION 1
#define ID 2

CControlUpdateDBase::CControlUpdateDBase(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CControlUpdateDBase)
{
	ui->setupUi(this);

	ui->wConnection->setConnectionType(CDataBaseConnectionWidget::ConnectionServer);
	ui->wConnection->setConnectionChoiceEnable(false);
	ui->wConnection->setCloseButtonVisible(false);
	connect(ui->wConnection, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

	ui->stackedWidget->setCurrentIndex(0);

	ui->twSelectAction->clear();
	ui->twSelectAction->hideColumn(ID);
}

CControlUpdateDBase::~CControlUpdateDBase()
{
	delete ui;
}

void CControlUpdateDBase::connected(QString connectionName)
{
	mConnectionName = connectionName;

	ui->stackedWidget->slideHorizontalNext();
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT id, name FROM Categories;"))
	{
		while(query.next())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem;
			if(item)
			{
				item->setText(CATEGORY, query.value(1).toString());

				QSqlQuery query1(QSqlDatabase::database(mConnectionName));
				if(query1.exec("SELECT id, title FROM Actions WHERE id_category=" + query.value(0).toString()))
				{
					while(query1.next())
					{
						QTreeWidgetItem *item1 = new QTreeWidgetItem;
						if(item1)
						{
							item1->setText(ID, query1.value(0).toString());
							item1->setText(ACTION, query1.value(1).toString());
							item->addChild(item1);
						}
					}
				}

				//Добавляются только не пустые категории:
				if(item->childCount() > 0)
					ui->twSelectAction->addTopLevelItem(item);
				else
					delete item;
			}
		}
		ui->twSelectAction->expandAll();
	}
}

void CControlUpdateDBase::on_pbnImport_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twSelectAction->currentItem();
	if(selectedItem && selectedItem->text(ID).isEmpty() == false)
	{
		static QString importPath;
		if(importPath.isEmpty())
			importPath = Global::currentPath() + "/import";
		QDir().mkpath(importPath);

		QString path = QFileDialog::getSaveFileName(this, tr("Укажите файл для сохранения иморта базы данных"), importPath + '/' + selectedItem->text(ACTION) + QDateTime::currentDateTime().toString("(yyyy_MM_dd hh-mm)") + ".db", tr("Файл базы данных(.db)"));
		if(!path.isEmpty())
		{
			QFileInfo fileInfo(path);
			if(fileInfo.completeSuffix() != "db")
				path = fileInfo.absolutePath() + '/' + fileInfo.baseName() + ".db";

			Uploading upl(mConnectionName);
			if(upl.openConnection(path))
			{
				upl.uploadingProcess(ui->twSelectAction->currentItem()->text(ID));
				QMessageBox::information(this, tr("Успех"), tr("Выгрузка базы данных мероприятия успешно завершена."));
			}
			else
				QMessageBox::warning(this, tr("Внимание"), tr("Не удалось открть базу данных."));
		}
	}
	else
		QMessageBox::warning(this, tr("Внимание"), tr("Не выбрано мероприятие для выгрузки."));
}
