#include "cstartingdialog.h"
#include "ui_cstartingdialog.h"

CStartingDialog::CStartingDialog(const QString connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CStartingDialog)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);

	mConnectionName = connectionName;
	mLogined = false;

	ui->wConnectionWidget->setConnectionName(mConnectionName);
	connect(ui->wConnectionWidget, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->wConnectionWidget, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

////	ui->wConnectionWidget->setConnectionChoiceEnable(false);
}

CStartingDialog::~CStartingDialog()
{
	delete ui;
}

void CStartingDialog::connected(const QString &connectionName)
{
	mConnectionName = connectionName;
	ui->stackedWidget->slideHorizontalNext();
}

void CStartingDialog::on_pbnExit_clicked()
{
	close();
}

void CStartingDialog::on_pbnLogin_clicked()
{
	if(mLogined)
		return;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT id, name, marketId FROM Users WHERE login = :login AND passwordCrypt = :password;");
	query.bindValue(":login", ui->leLogin->text());
	query.bindValue(":password", ui->lePassword->text());
	if(query.exec())
	{
		if(query.first())
		{
			ui->lStatus->setText(tr("Вход выполнен"));
			mLogined = true;
			mLoginedId = query.value(0).toInt();
			ui->lWelcome->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Добро пожаловать </span><span style=\" font-size:10pt; font-weight:600;\">%1</span><span style=\" font-size:10pt;\">!</span></p></body></html>").arg(query.value(1).toString()));
			CMarket::instance()->setMarketId(query.value(2).toInt());
			CMarket::instance()->setSeller(query.value(0).toInt(), query.value(1).toString());
			QTimer::singleShot(1000, this, SLOT(close()));
		}
		else
			ui->lStatus->setText(tr("Ошибка входа"));
	}
	else
		ui->lStatus->setText(tr("Ошибка"));
}

void CStartingDialog::on_leLogin_returnPressed()
{
	ui->lePassword->setFocus();
}

void CStartingDialog::on_lePassword_returnPressed()
{
	on_pbnLogin_clicked();
}
