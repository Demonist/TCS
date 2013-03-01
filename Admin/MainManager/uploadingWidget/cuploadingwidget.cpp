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
    //TODO! Проверка выбрано ли мероприятие
    QTreeWidgetItem *selectedItem = ui->twActions->currentItem();
    if(selectedItem && openConnection())
    {
        QSqlDatabase db = QSqlDatabase::database("uploadingConnection");
        createDBScheme();
        QSqlQuery selectDataQuery(QSqlDatabase::database(mConnectionName));
        if(selectDataQuery.exec("SELECT * FROM Actions WHERE id = " + ui->twActions->currentItem()->text(ID)))
        {
            QSqlQuery insertDataQuery(db);
            insertDataQuery.exec("INSERT INTO Actions VALUES(:id, :title, :performer, :description, :date, :state, :fanPrice, :fanCount, :id_place, :id_cat);");
            while(selectDataQuery.next())
            {
                insertDataQuery.bindValue(":id", selectDataQuery.value(0).toString());
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
        if(selectDataQuery.exec("SELECT * FROM Tickets WHERE id_action = " + ui->twActions->currentItem()->text(ID)))
        {
            QSqlQuery insertDataQuery(db);
            insertDataQuery.exec("INSERT INTO Tickets VALUES(NULL, :id_action, :id_placeScheme, :id_client, :identifier, :passedFlag);");
            while(selectDataQuery.next())
            {
                insertDataQuery.bindValue(":id_action", selectDataQuery.value(1).toString());
                insertDataQuery.bindValue(":id_placeScheme", selectDataQuery.value(2).toString());
                insertDataQuery.bindValue(":id_client", selectDataQuery.value(3).toString());
                insertDataQuery.bindValue(":identifier", selectDataQuery.value(4).toString());
                insertDataQuery.bindValue(":passedFlag", "false");
                insertDataQuery.exec();
                if(!selectDataQuery.value(3).isNull())
                {
                    QSqlQuery selectDataClients(QSqlDatabase::database(mConnectionName));
                    if(selectDataClients.exec("SELECT * FROM Clients WHERE id = " + selectDataQuery.value(3).toString()))
                    {
                        QSqlQuery insertDataClients(db);
                        insertDataClients.exec("INSERT INTO Clients VALUES(:id, :name, :birthDate, :login, :passwordHash, :clientsPhone)");
                        while(selectDataClients.next())
                        {
                            insertDataClients.bindValue(":id", selectDataClients.value(0).toString());
                            insertDataClients.bindValue(":name", selectDataClients.value(1).toString());
                            insertDataClients.bindValue(":birthDate", selectDataClients.value(2).toString());
                            insertDataClients.bindValue(":login", selectDataClients.value(3).toString());
                            insertDataClients.bindValue(":passwordHash", selectDataClients.value(4).toString());
                            insertDataClients.bindValue(":clientsPhone", selectDataClients.value(5).toString());
                            insertDataClients.exec();
                        }
                    }
                }
            }
        }
        QMessageBox::information(this, tr("Выгрузка успешно завершена"), tr("Выгрузка в файл ") + tr(" успешно завершена\nБаза валидна."));
        db.close();
    }
    else
    {
        //QMessageBox::critical(this, tr("Ошибка при открытии базы данных"), tr("Невозможно открыть базу данных, либо не выбрано мероприятие."));
    }
    QSqlDatabase::removeDatabase("uploadingConnection");
}

void CUploadingWidget::createDBScheme()
{
    QSqlDatabase db = QSqlDatabase::database("uploadingConnection");
    QSqlQuery createDBQuery(db);
    if(!createDBQuery.exec(
                "CREATE TABLE IF NOT EXISTS Actions( "
                "id                   INTEGER NOT NULL, "
                "title                TEXT NULL, "
                "performer            TEXT NULL, "
                "description          TEXT NULL, "
                "dateTime             DATETIME NULL, "
                "state                INTEGER DEFAULT 0, "
                "fanPrice             INTEGER DEFAULT 0, "
                "fanCount             INTEGER DEFAULT 0, "
                "id_place             INTEGER NULL, "
                "id_category          INTEGER NULL"
                "); "
                ))
        qDebug(qPrintable(createDBQuery.lastError().text()));
    if(!createDBQuery.exec(
                "CREATE TABLE IF NOT EXISTS Tickets( "
                "id                   INTEGER PRIMARY KEY AUTOINCREMENT, "
                "id_action            INTEGER NULL, "
                "id_placeScheme       INTEGER NULL, "
                "id_client            INTEGER NULL, "
                "identifier           TEXT NULL, "
                "passedFlag           BOOLEAN DEFAULT FALSE"
                ");"
                ))
        qDebug(qPrintable(createDBQuery.lastError().text()));
    if(!createDBQuery.exec(
                "CREATE TABLE IF NOT EXISTS Clients( "
                "id                   INTEGER NOT NULL, "
                "name                 TEXT NULL, "
                "birthDate            DATE NULL,  "
                "login                TEXT NULL,  "
                "passwordHash         TEXT NULL,  "
                "clientsPhone         TEXT NULL  "
                ");"
                ))
        qDebug(qPrintable(createDBQuery.lastError().text()));
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

bool CUploadingWidget::openConnection()
{
    if(validateDataBase())
    {
        QString pth = QFileDialog::getSaveFileName(this, tr("Сохранение базы мероприятия ") + tActionName, QDir::currentPath(), tr("Файл базы данных(.sqlite)"));
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "uploadingConnection");
        db.setDatabaseName(pth);
        if(db.open())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
         QMessageBox::critical(this, tr("База не валидна!"), tr("Для данного мероприятия исходная база данных не валидна.\nНайдено совпадение штрихкодов.\nПроверьте базу данных на совпадение штрихкодов."));
         return false;
    }
}

bool CUploadingWidget::validateDataBase()
{
    QSqlQuery vQuery(QSqlDatabase::database(mConnectionName));
    vQuery.exec("SELECT COUNT (*) AS mCount FROM Tickets WHERE id_action = " + ui->twActions->currentItem()->text(ID) + " GROUP BY identifier ORDER BY mCount DESC");
    vQuery.first();
    if(vQuery.value(0).toInt() > 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
