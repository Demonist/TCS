#include "cmarketswidget.h"
#include "ui_cmarketswidget.h"

#define ID 0
#define ADDRESS 1

CMarketsWidget::CMarketsWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CMarketsWidget)
{
    ui->setupUi(this);
    ui->twMarkets->hideColumn(ID);
    ui->twMarkets->setColumnWidth(ID, 0);
    ui->twMarkets->setColumnWidth(ADDRESS, 200);
}

CMarketsWidget::~CMarketsWidget()
{
    delete ui;
}

void CMarketsWidget::updateData()
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    if(query.exec("SELECT id, address FROM Markets;"))
    {
        ui->twMarkets->clear();
        uint count = 0;
        QTreeWidgetItem *item;
        while(query.next())
        {
            item = new QTreeWidgetItem();
            if(item)
            {
                item->setText(ID, query.value(0).toString());
                item->setText(ADDRESS, query.value(1).toString());
                ui->twMarkets->addTopLevelItem(item);
                count++;
            }
        }
        ui->groupBox->setTitle(tr("Торговые площадки: %1 шт").arg(count));
    }
}

void CMarketsWidget::on_tbnMarketAdd_clicked()
{
    CMarketDialog marketDialog(mConnectionName, this);
    connect(&marketDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
    marketDialog.exec();
}

void CMarketsWidget::on_tbnMarketEdit_clicked()
{
    QTreeWidgetItem *selectedItem = ui->twMarkets->currentItem();
    if(selectedItem)
    {
        CMarketDialog marketDialog(mConnectionName, selectedItem->text(ID).toInt(), this);
        connect(&marketDialog, SIGNAL(dataWasUpdated()), this, SLOT(updateData()));
        marketDialog.exec();
    }
}

void CMarketsWidget::on_tbnMarketDelete_clicked()
{

    QTreeWidgetItem *selectedItem = ui->twMarkets->currentItem();
    if(selectedItem)
    {
        if(QMessageBox::Yes == QMessageBox::question(this, tr("Запрос подтверждения"), tr("Вы действительно хотите удалить выбранную торговую площадку?"), QMessageBox::Yes, QMessageBox::No))
        {
            QSqlQuery query(QSqlDatabase::database(mConnectionName));
            query.prepare("DELETE FROM Markets WHERE id = :id");
            query.bindValue(":id", selectedItem->text(ID));
            query.exec();
            updateData();
        }
    }
}
