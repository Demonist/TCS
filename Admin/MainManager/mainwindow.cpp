#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectionWidget, SIGNAL(closed()), this, SLOT(close()));
    connect(ui->connectionWidget, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

////    ui->connectionWidget->setConnectionChoiceEnable(false);

    ui->sSettings->setStretchFactor(0, 1);
    ui->sSettings->setStretchFactor(2, 3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected(QString connectionName)
{
    QSqlQuery query(QSqlDatabase::database(connectionName));
    if(query.exec("SELECT COUNT(id) FROM serials") && query.first())
        qDebug("%d", query.value(0).toInt());
    else
        qDebug("alarme");
}
