#include "ccontrolupdatedbase.h"
#include "ui_ccontrolupdatedbase.h"

#define ACTION 0
#define ID 1

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
	ui->twSelectAction->hideColumn(1);
}

CControlUpdateDBase::~CControlUpdateDBase()
{
	delete ui;
}

void CControlUpdateDBase::connected(QString connectionName)
{
	mConnectionName = connectionName;
	QSqlDatabase db = QSqlDatabase::database(mConnectionName);
	if(!db.open())
	{
		QMessageBox::critical(this, tr("Ошибка в открытии базы данных!"), tr("База данных не открыта!\nОшибка в открытии базы данных!"));
	}
	else
	{
		ui->stackedWidget->slideHorizontalNext();
		QSqlQuery query(db);
		if(query.exec("SELECT id, name FROM Categories"))
		{
			while(query.next())
			{
				QTreeWidgetItem *item = new QTreeWidgetItem;
				if(item)
				{
					item->setText(ACTION, query.value(1).toString());
					QSqlQuery query1(db);
					if(query1.exec("SELECT id, title FROM Actions WHERE id_category=" + query.value(0).toString()))
					{
						while(query1.next())
						{
							QTreeWidgetItem *item1 = new QTreeWidgetItem;
							if(item1)
							{
								item1->setText(ACTION, query1.value(1).toString());
								item1->setText(ID, query1.value(0).toString());
								item->addChild(item1);
							}
						}
					}
					ui->twSelectAction->addTopLevelItem(item);
				}
			}
		}
	}
}

void CControlUpdateDBase::on_pbnExport_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twSelectAction->currentItem();
	if((!selectedItem->text(ID).isEmpty()))
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите экспортировать базу?"), QMessageBox::Yes, QMessageBox::No))
		{
			QString path = QFileDialog::getSaveFileName(this, tr("Сохранение базы мероприятия "), QDir::currentPath(), tr("Файл базы данных(.db)"));
			if(!path.isEmpty())
			{
				Uploading upl(mConnectionName);
				if(upl.openConnection(path))
				{
					upl.uploadingProcess(ui->twSelectAction->currentItem()->text(ID));
					QMessageBox::information(this, tr("Выгрузка успешно завершена"), tr("Выгрузка в файл ") + tr(" успешно завершена\nБаза валидна."));
				}
				else
					QMessageBox::critical(this, tr("Не удалось открыть базу данных!"), tr("Не удалось открть базу данных."));

			}
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Ошибка выгрузки"), tr("Не выбрано мероприятие."));
	}
}
