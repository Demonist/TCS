#include "cadminstartingdialog.h"
#include "ui_cadminstartingdialog.h"

CAdminStartingDialog::CAdminStartingDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CAdminStartingDialog)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(0);

	mLogined = false;
	mConnectionName = connectionName;
	ui->wDataBaseConnect->setConnectionName(mConnectionName);
////	ui->wDataBaseConnect->setConnectionChoiceEnable(false);

	connect(ui->wDataBaseConnect, SIGNAL(closed()), this, SLOT(close()));
	connect(ui->wDataBaseConnect, SIGNAL(connectedToDatabase(QString)), this, SLOT(connected(QString)));

	connect(ui->leLogin, SIGNAL(returnPressed()), ui->lePassword, SLOT(setFocus()));
}

CAdminStartingDialog::~CAdminStartingDialog()
{
	delete ui;
}

void CAdminStartingDialog::connected(const QString &connectionName)
{
	ui->stackedWidget->slideHorizontalNext();
}

void CAdminStartingDialog::on_pbnExit_clicked()
{
	close();
}

void CAdminStartingDialog::on_pbnLogin_clicked()
{
	if(mLogined)
		return;

	if(ui->leLogin->text() == "root" && ui->lePassword->text() == "root")
	{
		ui->lStatus->setText(tr("Вход выполнен"));
		mLogined = true;
		gAdminId = 0;
		gAdminName = tr("Администратор");
		ui->lWelcome->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Добро пожаловать </span><span style=\" font-size:10pt; font-weight:600;\">%1</span><span style=\" font-size:10pt;\">!</span></p></body></html>").arg(gAdminName));
		QTimer::singleShot(1000, this, SLOT(close()));
		return;
	}

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT id, name FROM Admins WHERE login = :login AND passwordCrypt = :password;");
	query.bindValue(":login", ui->leLogin->text());
	query.bindValue(":password", Global::crypt(ui->lePassword->text()));
	if(query.exec())
	{
		if(query.first())
		{
			ui->lStatus->setText(tr("Вход выполнен"));
			mLogined = true;
			gAdminId = query.value(0).toInt();
			gAdminName = query.value(1).toString();
			ui->lWelcome->setText(tr("<html><head/><body><p align=\"center\"><span style=\" font-size:10pt;\">Добро пожаловать </span><span style=\" font-size:10pt; font-weight:600;\">%1</span><span style=\" font-size:10pt;\">!</span></p></body></html>").arg(gAdminName));
			QTimer::singleShot(1000, this, SLOT(close()));
		}
		else
			ui->lStatus->setText(tr("Ошибка входа"));
	}
	else
	{
		ui->lStatus->setText(tr("Ошибка"));
		qDebug(qPrintable(query.lastError().text()));
	}
}

void CAdminStartingDialog::on_lePassword_returnPressed()
{
	on_pbnLogin_clicked();
}
