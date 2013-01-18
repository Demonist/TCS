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
    if(query.exec("SELECT Actions.ID, Actions.Title, Categories.name FROM Categories, Actions WHERE Actions.id_category = Categories.ID;"))
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
    if(tDate == "")
    {
        tActionName = selectedItem->text(NAME);
        ui->lInfo->setText(tr("Выгрузка мероприятия: ")+selectedItem->text(NAME));
    }
    else
    {
        tActionName = selectedItem->text(NAME);
        ui->lInfo->setText(tr("Выгрузка мероприятия: ")+selectedItem->text(NAME) + tr(", начиная с ") + tDate);
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

void CUploadingWidget::on_tbnSearch_clicked()
{

    QTreeWidgetItemIterator it(ui->twActions);
    while (*it)
    {
        if (!(*it)->text(1).contains(ui->leSearch->text(), Qt::CaseInsensitive) && ((*it)->childCount() == 0))
        {
            (*it)->setHidden(true);
        }
        else
        {
            (*it)->setHidden(false);
        }
        ++it;
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

}
