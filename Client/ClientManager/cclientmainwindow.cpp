#include "cclientmainwindow.h"
#include "ui_cclientmainwindow.h"

CClientMainWindow::CClientMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CClientMainWindow)
{
	ui->setupUi(this);

	ui->splitter->setCollapsible(0, true);
	ui->splitter->setStretchFactor(0, 1);
	ui->splitter->setStretchFactor(1, 100);

	mConnectionName = "dataBaseClient";

	CStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();

	if(startingDialog.isLogined())
	{
		ui->wActions->setConnectionName(mConnectionName);
		show();
	}
	else
		QTimer::singleShot(0, this, SLOT(close()));	//Обычный вызов close(); не срабатывает т.к. eventLoop еще не запущен.
}

CClientMainWindow::~CClientMainWindow()
{
	delete ui;

	{
		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		db.close();
	}
	QSqlDatabase::removeDatabase(mConnectionName);
}
