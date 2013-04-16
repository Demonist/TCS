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
		mAdvertisementUpdateTimer.stop();
		mActionDialog.setCanClose(true);
		mActionDialog.close();
		delete CImages::instance();
		delete CStatistics::instance();
		delete CMarket::instance();
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

	setWindowIcon(QIcon(":/AppIcon.ico"));

	connect(ui->wActions, SIGNAL(hideLeftPanel()), this, SLOT(hideLeftPanel()));
	connect(ui->wActions, SIGNAL(showLeftPanel()), this, SLOT(showLeftPanel()));

	mConnectionName = "dataBaseClient";
	mCanClose = true;

	CStartingDialog startingDialog(mConnectionName, this);
	startingDialog.exec();

	if(startingDialog.isLogined())
	{
		qsrand((uint)QDateTime::currentMSecsSinceEpoch());
		CImages::instance(mConnectionName);
		CStatistics::instance(mConnectionName);
		ui->wActions->setConnectionName(mConnectionName);
		ui->wReservation->setConnectionName(mConnectionName);
		ui->wReturnTicket->setConnectionName(mConnectionName);
        ui->wRegistration->setConnectionName(mConnectionName);
		ui->leUser->setText(CMarket::instance()->seller());
		ui->leUser->setCursorPosition(0);

		mActionDialog.setSourceView(ui->wActions->view());
		mActionDialog.show();
		connect(ui->wActions, SIGNAL(hideLeftPanel()), &mActionDialog, SLOT(showSeats()));
		connect(ui->wActions, SIGNAL(showLeftPanel()), &mActionDialog, SLOT(showAdvertisements()));

		mAdvertisementUpdateTimer.setSingleShot(false);
		mAdvertisementUpdateTimer.setInterval(600000);	// 10 min.
		connect(&mAdvertisementUpdateTimer, SIGNAL(timeout()), this, SLOT(updateAdvertisement()));
		mAdvertisementUpdateTimer.start();
		updateAdvertisement();

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

//private slots:

void CClientMainWindow::hideLeftPanel()
{
	ui->gbxUser->hide();
	ui->listWidget->hide();
	ui->pbnAbout->hide();
}

void CClientMainWindow::showLeftPanel()
{
	ui->listWidget->show();
	ui->gbxUser->show();
	ui->pbnAbout->show();
}

void CClientMainWindow::on_listWidget_currentRowChanged(int currentRow)
{
	switch(currentRow)
	{
		case 0: ui->wActions->updateData(); break;
		case 1: ui->wReservation->updateData(); break;
		case 3: ui->wRegistration->updateData(); break;
	}

	ui->wActions->showActions();
	ui->stackedWidget->setCurrentIndexAnimatedVertical(currentRow);
}

void CClientMainWindow::on_pbnChangeUser_clicked()
{
	CStartingDialog startingDialog(mConnectionName, this);
	startingDialog.setChangeUserOnly();
	startingDialog.exec();
	if(startingDialog.isLogined())
	{
		ui->leUser->setText(CMarket::instance()->seller());
		ui->leUser->setCursorPosition(0);
	}
}

void CClientMainWindow::updateAdvertisement()
{
	mActionDialog.updateAdvertisements(mConnectionName);
	ui->wActions->updateData();
}

void CClientMainWindow::on_pbnAbout_clicked()
{
	CAboutDialog aboutDialog(this);
	aboutDialog.exec();
}
