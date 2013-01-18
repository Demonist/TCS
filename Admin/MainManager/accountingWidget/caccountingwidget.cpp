#include "caccountingwidget.h"
#include "ui_caccountingwidget.h"

CAccountingWidget::CAccountingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CAccountingWidget)
{
    ui->setupUi(this);
}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
    ui->cbxMarkets->clear();
    ui->cbxUsers->clear();
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    if(query.exec("SELECT ID, address From Markets;"))
    {
        while(query.next())
        {
            ui->cbxMarkets->addItem(query.value(1).toString(), query.value(0).toString());
        }

    }
    else qDebug(qPrintable(query.lastError().text()));

    if(query.exec("SELECT ID, name From Users;"))
    {
        while(query.next())
        {
            ui->cbxUsers->addItem(query.value(1).toString(), query.value(0).toString());
        }

    }
    else qDebug(qPrintable(query.lastError().text()));
}

void CAccountingWidget::on_tbnActions_clicked()
{
    CAccountingActionsDialog caccountingactionsdialog(mConnectionName, this);
    caccountingactionsdialog.exec();
    if(caccountingactionsdialog.result())
    {

    }
}

void CAccountingWidget::on_tbnS_clicked()
{
    CDateDialog dateDialog(this);
    if(ui->leSDate->text().isEmpty() == false)
        dateDialog.setDate(QDate::fromString(ui->leSDate->text(), "dd.MM.yyyy"));
    dateDialog.exec();
    if(dateDialog.result())
        ui->leSDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
}

void CAccountingWidget::on_tbnDo_clicked()
{
    CDateDialog dateDialog(this);
    if(ui->leDoDate->text().isEmpty() == false)
        dateDialog.setDate(QDate::fromString(ui->leDoDate->text(), "dd.MM.yyyy"));
    dateDialog.exec();
    if(dateDialog.result())
        ui->leDoDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
}
