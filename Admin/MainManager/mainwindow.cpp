#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->swSettings->setCurrentIndex(0);

	setWindowIcon(QIcon(":/AppIcon.ico"));

	ui->sSettings->setCollapsible(0, true);
	ui->sSettings->setStretchFactor(0, 1);
	ui->sSettings->setStretchFactor(1, 100);

	mConnectionName = "dataBaseAdmin";
	CAdminStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();
	if(startingDialog.isLogined())
	{
		ui->wUsers->setConnectionName(mConnectionName);
		ui->wCategories->setConnectionName(mConnectionName);
		ui->wPlaces->setConnectionName(mConnectionName);
		ui->wClients->setConnectionName(mConnectionName);
		ui->wActions->setConnectionName(mConnectionName);
		ui->wMarkets->setConnectionName(mConnectionName);
		ui->wAccounting->setConnectionName(mConnectionName);
		ui->wUploading->setConnectionName(mConnectionName);
		ui->wAdvertisements->setConnectionName(mConnectionName);

		CImages::instance(mConnectionName);

		showMaximized();
	}
	else
		QTimer::singleShot(0, this, SLOT(close()));	//Обычный вызов close(); не срабатывает т.к. eventLoop еще не запущен.
}

MainWindow::~MainWindow()
{
	delete ui;

	{
		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		db.close();
	}
	QSqlDatabase::removeDatabase(mConnectionName);
}

void MainWindow::on_lwSettings_currentRowChanged(int currentRow)
{
	switch(currentRow)
	{
		case 0: ui->wUsers->updateData(); break;
		case 1: ui->wCategories->updateData(); break;
		case 2: ui->wPlaces->updateData(); break;
		case 3: ui->wClients->updateData(); break;
		case 4: ui->wActions->updateData(); break;
		case 5: ui->wMarkets->updateData(); break;
		case 6: ui->wAccounting->updateData(); break;
		case 7: ui->wUploading->updateData(); break;
		case 8: ui->wAdvertisements->updateData(); break;
	}

	ui->swSettings->setCurrentIndexAnimatedVertical(currentRow);
}
