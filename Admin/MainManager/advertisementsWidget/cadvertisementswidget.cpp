#include "cadvertisementswidget.h"
#include "ui_cadvertisementswidget.h"

CAdvertisementsWidget::CAdvertisementsWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CAdvertisementsWidget)
{
	ui->setupUi(this);
}

CAdvertisementsWidget::~CAdvertisementsWidget()
{
	delete ui;
}

void CAdvertisementsWidget::updateData()
{
	static CCacheChecker cacheChecker("Advertisements", mConnectionName);
	if(cacheChecker.isNeedUpdate())
	{
		cacheChecker.setUpdated();

		QSqlQuery query(QSqlDatabase::database(mConnectionName));

		if(query.exec("SELECT id FROM Advertisements;"))
		{
			int counter = 0;
			QListWidgetItem *item;
			CImages *images = CImages::instance();
			if(!images)
			{
				qCritical("CAdvertisementsWidget::updateData: No images handle!");
				return;
			}

			ui->lwImages->clear();

			while(query.next())
			{
				item = new QListWidgetItem;
				if(item)
				{
					item->setData(Qt::UserRole, query.value(0));
					item->setIcon(QIcon(images->image(query.value(0).toInt())));
					ui->lwImages->addItem(item);
				}
				counter++;
			}
			ui->gbxAdvertisement->setTitle(tr("Реклама: %1 шт.").arg(counter));
		}
	}
}

void CAdvertisementsWidget::on_tbnAdd_clicked()
{
	CImageDialog imageDialog(mConnectionName, this);
	imageDialog.setWindowTitle(tr("Изображение для рекламы"));
	if(imageDialog.exec() == QDialog::Accepted)
	{
		QSqlQuery query(QSqlDatabase::database(mConnectionName));
		query.prepare("INSERT INTO Advertisements VALUES(:id);");
		query.bindValue(":id", imageDialog.id());
		if(query.exec())
			updateData();
		else
			qDebug(qPrintable(query.lastError().text()));
	}
}

void CAdvertisementsWidget::on_tbnDel_clicked()
{
	QListWidgetItem *selected = ui->lwImages->selectedItems().value(0);
	if(selected)
	{
		if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную рекламу?"), QMessageBox::Yes, QMessageBox::No))
		{
			QSqlQuery query(QSqlDatabase::database(mConnectionName));
			query.prepare("DELETE FROM Advertisements WHERE id = :id;");
			query.bindValue(":id", selected->data(Qt::UserRole));
			if(query.exec())
				updateData();
			else
				qDebug(qPrintable(query.lastError().text()));
		}
	}
}
