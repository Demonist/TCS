#include "caccountingwidget.h"
#include "ui_caccountingwidget.h"

#define RETURNED_MARKET 0
#define RETURNED_SELLER 1
#define RETURNED_ACTION 2

#define SPT_ACTION_ID 0
#define SPT_ACTION_NAME 1

CAccountingWidget::CAccountingWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CAccountingWidget)
{
    ui->setupUi(this);

    ui->twActions->setColumnWidth(SPT_ACTION_ID, 0);

}

CAccountingWidget::~CAccountingWidget()
{
    delete ui;
}

void CAccountingWidget::updateData()
{
    sumPriceTicketsForAction();

    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.prepare("SELECT data FROM Statistics WHERE type = :type;");
    query.bindValue(":type", CStatistics::TypeTicketReturned);
    if(query.exec())
    {
        QTreeWidgetItem *item;
        while(query.next())
        {
            CStatisticTicketReturnedType *type = (CStatisticTicketReturnedType*)CStatistics::fromByteArray(query.value(0).toByteArray());
            if(type && type->isValid())
            {
                item = new QTreeWidgetItem();
                if(item)
                {
                    QSqlQuery query2(QSqlDatabase::database(mConnectionName));
                    query2.prepare("SELECT Markets.address, Users.name, Actions.title FROM Markets, Users, Actions WHERE Markets.id = :marketId AND Users.id = :userId AND Actions.id = :actId;");
                    query2.bindValue(":marketId", type->marketId);
                    query2.bindValue(":userId", type->sellerId);
                    query2.bindValue(":actId", type->actionId);
                    if(query2.exec() && query2.first())
                    {
                        item->setText(RETURNED_MARKET, query2.value(0).toString());
                        item->setText(RETURNED_SELLER, query2.value(1).toString());
                        item->setText(RETURNED_ACTION, query2.value(2).toString());
                        ui->twReturnedTickets->addTopLevelItem(item);
                    }
                    else
                        qDebug(qPrintable(query2.lastError().text()));
                }
            }
        }
    }
    else
        qDebug(qPrintable(query.lastError().text()));
}

void CAccountingWidget::sumPriceTicketsForAction()
{
    ui->cbxCategoriesSelect->clear();
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    if(query.exec("SELECT id, name FROM Categories;"))
    {
        while(query.next())
        {
            ui->cbxCategoriesSelect->addItem(query.value(1).toString(), query.value(0).toString());
        }
    }
}

void CAccountingWidget::on_cbxCategoriesSelect_currentIndexChanged(int index)
{
    ui->twActions->clear();
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.prepare("SELECT id, title FROM Actions WHERE id_category = :id;");
    query.bindValue(":id", ui->cbxCategoriesSelect->itemData(index));
    if(query.exec())
    {
        QTreeWidgetItem *item;
        while(query.next())
        {
            item = new QTreeWidgetItem();
            if(item)
            {
                item->setText(SPT_ACTION_ID, query.value(0).toString());
                item->setText(SPT_ACTION_NAME, query.value(1).toString());
                ui->twActions->addTopLevelItem(item);
            }
        }
    }
}
