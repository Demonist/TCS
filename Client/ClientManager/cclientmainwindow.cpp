#include "cclientmainwindow.h"
#include "ui_cclientmainwindow.h"

CClientMainWindow::CClientMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CClientMainWindow)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->splitter->setCollapsible(0, true);
	ui->splitter->setStretchFactor(0, 1);
	ui->splitter->setStretchFactor(1, 100);

	mConnectionName = "dataBaseClient";

	CStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();

	if(startingDialog.isLogined())
	{
		CImages::instance(mConnectionName);
		ui->wActions->setConnectionName(mConnectionName);
		ui->wSceneActions->setConnectionName(mConnectionName);
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

void CClientMainWindow::on_listWidget_currentRowChanged(int currentRow)
{
	switch(currentRow)
	{
		case 0: ui->wActions->updateData(); break;
		case 1: ui->wSceneActions->updateData(); break;
	}

	ui->stackedWidget->setCurrentIndexAnimatedVertical(currentRow);
}
