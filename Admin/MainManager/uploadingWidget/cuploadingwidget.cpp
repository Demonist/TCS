#include "cuploadingwidget.h"
#include "ui_cuploadingwidget.h"

#define ID 0
#define NAME 1
#define TYPE 2
#define STATE 3

CUploadingWidget::CUploadingWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CUploadingWidget)
{
	ui->setupUi(this);
	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);
	ui->twActions->setColumnWidth(NAME, 300);
	ui->twActions->setColumnWidth(TYPE, 150);
	ui->twActions->setColumnWidth(STATE, 150);
}

CUploadingWidget::~CUploadingWidget()
{
	delete ui;
}

void CUploadingWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT Actions.id, Actions.title, Categories.name, Actions.state FROM Categories, Actions WHERE Actions.id_category = Categories.id;"))
	{
		ui->twActions->clear();
		QTreeWidgetItem *item;
		while(query.next())
		{
			item = new QTreeWidgetItem();
			if(item)
			{
				item->setText(ID, query.value(0).toString());
				item->setText(NAME, query.value(1).toString());
				item->setText(TYPE, query.value(2).toString());
				item->setText(STATE, Global::actionStateToText(query.value(3).toInt()));

				ui->twActions->addTopLevelItem(item);
			}
		}
	}
}

void CUploadingWidget::on_twActions_itemSelectionChanged()
{
	QTreeWidgetItem *selectedItem = ui->twActions->currentItem();
	if(selectedItem)
	{
		tActionName = selectedItem->text(NAME);

		QString text = tr("Выгрузка мероприятия: ")+selectedItem->text(NAME);
		if(tDate.isEmpty() == false)
			text += tr(", начиная с ") + tDate;
		ui->lInfo->setText(text);
	}
}

void CUploadingWidget::on_tbnSelectDate_clicked()
{
	CDateDialog dateDialog(this);
	if(ui->leDate->text().isEmpty() == false)
	dateDialog.setDate(QDate::fromString(ui->leDate->text(), "dd.MM.yyyy"));
	dateDialog.exec();
	if(dateDialog.result())
	{
		if(tActionName == "")
		{
			tDate = dateDialog.selectedDate().toString("dd.MM.yyyy");
			ui->leDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
		}
		else
		{
			tDate = dateDialog.selectedDate().toString("dd.MM.yyyy");
			ui->leDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
			ui->lInfo->setText(tr("Выгрузка мероприятия: ")+ tActionName + tr(", начиная с ") + tDate);
		}

	}
}

void CUploadingWidget::on_tbClearDate_clicked()
{
	tDate = "";
	ui->leDate->setText("");
	ui->lInfo->setText(tr("Выгрузка мероприятия: ")+tActionName);
}

void CUploadingWidget::on_pbnUploading_clicked()
{
	QTreeWidgetItem *selectedItem = ui->twActions->currentItem();
	if(selectedItem)
	{

		QString pth = QFileDialog::getSaveFileName(this, tr("Сохранение базы мероприятия ") + tActionName, QDir::currentPath(), tr("Файл базы данных(.db)"));
		Uploading upl(mConnectionName, pth, ui->twActions->currentItem()->text(ID));
		if(upl.openConnection())
		{
			upl.uploadingProcess();
			QMessageBox::information(this, tr("Выгрузка успешно завершена"), tr("Выгрузка в файл ") + tr(" успешно завершена\nБаза валидна."));
		}
		else
		{
			QMessageBox::critical(this, tr("Не удалось открыть базу данных!"), tr("Не удалось открыть базу данных."));
		}
	}
	else
	{
		QMessageBox::critical(this, tr("Ошибка выгрузки"), tr("Не выбрано мероприятие."));
	}
}

void CUploadingWidget::on_tbnSearchClear_clicked()
{
	ui->leSearch->clear();
}

void CUploadingWidget::on_leSearch_textChanged(const QString &text)
{
	QTreeWidgetItem *item;
	for(int i = 0; i < ui->twActions->topLevelItemCount(); i++)
	{
		item = ui->twActions->topLevelItem(i);

		if(text.isEmpty())
			item->setHidden(false);
		else if(item->text(NAME).contains(text, Qt::CaseInsensitive))
			item->setHidden(false);
		else
			item->setHidden(true);
	}
}
