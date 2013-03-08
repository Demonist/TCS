#include "controlmanagermainwindow.h"
#include "ui_controlmanagermainwindow.h"

ControlManagerMainWindow::ControlManagerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlManagerMainWindow)
{
    ui->setupUi(this);
    ui->wConnection->setConnectionType(CDataBaseConnectionWidget::ConnectionFile);
    ui->wConnection->setConnectionChoiceEnable(false);
    connect(ui->aExit, SIGNAL(activated()), this, SLOT(close()));
    connect(ui->wConnection, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));
    ui->stackedWidget->setCurrentIndex(0);
}

ControlManagerMainWindow::~ControlManagerMainWindow()
{
    delete ui;
}

void ControlManagerMainWindow::on_leGetBarcode_returnPressed()
{
    QSqlDatabase db = QSqlDatabase::database(mConnectionName);
    if(db.isOpen() && db.isValid())
    {
        CTicketIdentifier *ticketIdentifier = new CTicketIdentifier(ui->leGetBarcode->text());
        QString barcode = ticketIdentifier->data();
        QSqlQuery query(db);
        query.prepare("SELECT COUNT(identifier), passedFlag, id FROM Tickets WHERE identifier = :identifier;");
        query.bindValue(":identifier", barcode);
        if(query.exec() && query.first())
        {
            if((query.value(0).toInt() == 1) && (query.value(1).toBool() == false))
            {
                ui->wOutResult->setStyleSheet("background: green");
                QTreeWidgetItem *item = new QTreeWidgetItem;
                if(item)
                {
                    item->setText(0, ui->leGetBarcode->text() + tr(" - Билет валиден"));
                    item->setBackgroundColor(0, QColor(131, 255, 131));
                    ui->twBarcodeControl->addTopLevelItem(item);
                    QSqlQuery setTrueFlag(db);
                    setTrueFlag.exec("UPDATE Tickets SET passedFlag = 'true' WHERE id = " + query.value(2).toString());
                }
            }
            else if((query.value(0).toInt() == 1) && (query.value(1).toBool() == true))
            {
                ui->wOutResult->setStyleSheet("background: yellow");
                QTreeWidgetItem *item = new QTreeWidgetItem;
                if(item)
                {
                    item->setText(0, ui->leGetBarcode->text() + tr(" - Попытка повторного прохода"));
                    item->setBackgroundColor(0, QColor(255, 240, 40));
                    ui->twBarcodeControl->addTopLevelItem(item);
                }
            }
            else if((query.value(0).toInt() > 1))
            {
                QMessageBox::critical(this, tr("Ошибка в базе данных!"), tr("В базе данных найдено несколько одинаковых штрихкодов (")+query.value(0).toString()+tr(")!"));
            }
            else
            {
                //TODO: Запрос к таблице возвращенных, если там есть, то предупреждение о мошеннике -> на кол(записать фио, логин и тд в лог), если нет -> на кол.
                if(1 == 2/* Запрос к таблице сданных билетов */)
                {
                    //Попытка пройти по сданному билету..45325431
                }
                else
                {
                    ui->wOutResult->setStyleSheet("background: red");
                    QTreeWidgetItem *item = new QTreeWidgetItem;
                    if(item)
                    {
                        item->setText(0, ui->leGetBarcode->text() + tr(" - Попытка пройти по несуществующему билету"));
                        item->setBackgroundColor(0, QColor(255, 131, 131));
                        ui->twBarcodeControl->addTopLevelItem(item);
                    }
                }
            }

        }
        else
        {
            QMessageBox::critical(this, tr("Ошибка выполнения запроса."), tr("Ошибка выполнения запроса.\nБаза данных не валидна."));
        }
        ui->leGetBarcode->setText("");
    }
    else
    {
        QMessageBox::critical(this, tr("База данных не открыта!"), tr("База данных не открыта!\nОткройте базу данных"));
        ui->leGetBarcode->setText("");
    }
}

void ControlManagerMainWindow::connected(QString connectionName)
{
    mConnectionName = connectionName;
    QSqlDatabase db = QSqlDatabase::database(mConnectionName);

    if(!db.open())
    {
        QMessageBox::critical(this, tr("Ошибка в открытии базы данных!"), tr("База данных не открыта!\nОшибка в открытии базы данных!"));
    }
    else
    {
        QSqlQuery query(db);
        query.exec("SELECT COUNT (*) AS mCount FROM Tickets GROUP BY identifier ORDER BY mCount DESC");
        if(query.first())
        {
            if(query.value(0).toInt() > 1)
            {
                  QMessageBox::critical(this, tr("База не валидна!"), tr("База данных не валидна.\nНайдено совпадение штрихкодов"));
                  db.close();
            }
            else
            {
                query.prepare("SELECT title, performer, dateTime FROM Actions");
                if(query.exec() && query.first())
                {
                   ui->stackedWidget->setCurrentIndex(1);
                   ui->groupBox->setTitle(tr("База данных концерта: ") + query.value(0).toString() + tr(", Исполнитель: ") + query.value(1).toString() + tr(", Дата проведения концерта: ") + query.value(2).toString());
                   ui->aAddThisDB->setEnabled(true);
                }
            }
        }
    }
}
