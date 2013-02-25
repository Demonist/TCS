#include "cclientmainwindow.h"
#include "ui_cclientmainwindow.h"

//private:

void CClientMainWindow::closeEvent(QCloseEvent *event)
{
	if(mCanClose)
	{
		event->accept();
		return;
	}

	if(QMessageBox::Yes == QMessageBox::question(this, tr("Хотите выйти?"), tr("Вы действительно хотите завершить работу данной программы?"), QMessageBox::Yes, QMessageBox::No))
	{
		mActionDialog.setCanClose(true);
		mActionDialog.close();
		event->accept();
	}
	else
		event->ignore();
}


//public:

CClientMainWindow::CClientMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CClientMainWindow),
	mActionDialog(this)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);
	ui->splitter->setCollapsible(0, true);
	ui->splitter->setStretchFactor(0, 1);
	ui->splitter->setStretchFactor(1, 100);

	connect(ui->wActions, SIGNAL(hideLeftPanel()), ui->listWidget, SLOT(hide()));
	connect(ui->wActions, SIGNAL(showLeftPanel()), ui->listWidget, SLOT(show()));

	mConnectionName = "dataBaseClient";
	mCanClose = true;

	CStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();

	if(startingDialog.isLogined())
	{
		qsrand(QTime::currentTime().msec());
		CImages::instance(mConnectionName);
		ui->wActions->setConnectionName(mConnectionName);
        ui->wRegistration->setConnectionName(mConnectionName);

		mActionDialog.setSourceView(ui->wActions->view());
		mActionDialog.showMinimized();

		showMaximized();
		mCanClose = false;
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
		case 3: ui->wRegistration->updateData(); break;
	}

	ui->wActions->showActions();
	ui->stackedWidget->setCurrentIndexAnimatedVertical(currentRow);
}
