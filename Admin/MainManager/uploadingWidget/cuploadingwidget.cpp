#include "cuploadingwidget.h"
#include "ui_cuploadingwidget.h"

#define ID 0
#define NAME 1
#define TYPE 2

CUploadingWidget::CUploadingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CUploadingWidget)
{
    ui->setupUi(this);
    ui->twActions->hideColumn(ID);
    ui->twActions->setColumnWidth(ID, 0);
    ui->twActions->setColumnWidth(NAME, 200);
    ui->twActions->setColumnWidth(TYPE, 200);
}

CUploadingWidget::~CUploadingWidget()
{
    delete ui;
}

void CUploadingWidget::updateData()
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT Actions.id, Actions.title, Categories.name FROM Categories, Actions WHERE Actions.id_category = Categories.id;"))
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
    QString pth = QFileDialog::getSaveFileName(this, tr("Сохранение базы мероприятия ") + tActionName, QDir::currentPath(), tr("Файл базы данных(.sqlite)"));
    QFile file(pth);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.close();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "myConnection");
    db.setDatabaseName(pth);
    if(db.open())
    {
        QSqlQuery createDBQuery(db);
        createDBQuery.exec(createDBScheme());
        QSqlQuery selectDataQuery(QSqlDatabase::database(mConnectionName));
        if(selectDataQuery.exec("SELECT * FROM Actions WHERE id = " + ui->twActions->currentItem()->text(ID)))
        {
            QSqlQuery insertDataQuery(db);
            insertDataQuery.exec("INSERT INTO Actions VALUES(NULL, :title, :performer, :description, :date, :state, :fanPrice, :fanCount, :id_place, :id_cat);");
            while(selectDataQuery.next())
            {
                insertDataQuery.bindValue(":title", selectDataQuery.value(1).toString());
                insertDataQuery.bindValue(":performer", selectDataQuery.value(2).toString());
                insertDataQuery.bindValue(":description", selectDataQuery.value(3).toString());
                insertDataQuery.bindValue(":date", selectDataQuery.value(4).toString());
                insertDataQuery.bindValue(":state", selectDataQuery.value(5).toString());
                insertDataQuery.bindValue(":fanPrice", selectDataQuery.value(6).toString());
                insertDataQuery.bindValue(":fanCount", selectDataQuery.value(7).toString());
                insertDataQuery.bindValue(":id_place", selectDataQuery.value(8).toString());
                insertDataQuery.bindValue(":id_cat", selectDataQuery.value(9).toString());
                insertDataQuery.exec();
            }



        }


    }
}

QString CUploadingWidget::createDBScheme()
{
    return "CREATE TABLE IF NOT EXISTS Actions( "
                       "id                   INTEGER PRIMARY KEY AUTOINCREMENT, "
                       "title                TEXT NULL, "
                       "performer            TEXT NULL, "
                       "description          TEXT NULL, "
                       "dateTime             DATETIME NULL, "
                       "state                INTEGER DEFAULT 0, "
                       "fanPrice             INTEGER DEFAULT 0, "
                       "fanCount             INTEGER DEFAULT 0, "
                       "id_place             INTEGER NULL, "
                       "id_category          INTEGER NULL"
                       ");";

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
