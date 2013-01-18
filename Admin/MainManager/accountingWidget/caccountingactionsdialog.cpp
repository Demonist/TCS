#include "caccountingactionsdialog.h"
#include "ui_caccountingactionsdialog.h"

#define ID 0
#define TITLE 1
#define STATE 2

CAccountingActionsDialog::CAccountingActionsDialog(const QString &connectionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAccountingActionsDialog)
{
    ui->setupUi(this);

    ui->twActions->hideColumn(ID);
    ui->twActions->setColumnWidth(ID, 0);
    ui->twActions->setColumnWidth(TITLE, 200);
    ui->twActions->setColumnWidth(STATE, 200);


    mConnectionName = connectionName;
    setWindowTitle(tr("Добавление категории"));

    QSqlQuery query(QSqlDatabase::database(mConnectionName));

    if(query.exec("SELECT ID, title, state From Actions;"))
    {
        QTreeWidgetItem *item;
        while(query.next())
        {
            item = new QTreeWidgetItem();
            if(item)
            {
                item->setText(ID, query.value(0).toString());
                item->setText(TITLE, query.value(1).toString());
                item->setText(STATE, query.value(2).toString());

                ui->twActions->addTopLevelItem(item);
                //count++;
            }
        }

    }
}

CAccountingActionsDialog::~CAccountingActionsDialog()
{
    delete ui;
}
