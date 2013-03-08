#include "cclientregistrationwidget.h"
#include "ui_cclientregistrationwidget.h"

CClientRegistrationWidget::CClientRegistrationWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CClientRegistrationWidget)
{
    ui->setupUi(this);
}

CClientRegistrationWidget::~CClientRegistrationWidget()
{
    delete ui;
}


void CClientRegistrationWidget::updateData()
{

}

void CClientRegistrationWidget::on_pbnSave_clicked()
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    if(validateLogin(ui->leLogin->text()))
    {
		QString password = generatePassword();

		CStatisticClientRegisteredType type;
		type.marketId = CMarket::instance()->marketId();
		type.sellerId = CMarket::instance()->sellerId();
		type.clientName = ui->leFIO->text();
		type.clientBirthDate = ui->cwBDate->selectedDate().toString("dd.MM.yyyy");
		type.clientLogin = ui->leLogin->text();
		type.clientPassword = password;
		type.clientPhone = ui->lePhone->text();

		query.prepare("INSERT INTO Clients VALUES(NULL, :name, :birthDate, :login, :passwordHash, :phone);");
        query.bindValue(":name", ui->leFIO->text());
		query.bindValue(":birthDate", ui->cwBDate->selectedDate());
        query.bindValue(":login", ui->leLogin->text());
		query.bindValue(":passwordHash", Global::crypt(password));
		query.bindValue(":phone", ui->lePhone->text());
		if(query.exec())
		{
			CStatistics::instance()->write(type);
			QMessageBox::information(this, tr("Успех"), tr("Новый пользователь успешно зарегистрирован.\nЕго пароль: %1").arg(password));

			ui->leFIO->clear();
			ui->leLogin->clear();
			ui->lePhone->clear();			
		}
		else
			qDebug(qPrintable(query.lastError().text()));
    }
    else
		QMessageBox::warning(this, tr("Внимание"), tr("Пользователь с таким логином уже существует!"));
}

bool CClientRegistrationWidget::validateLogin(QString login)
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT COUNT(id) FROM Clients WHERE login = :login;");
	query.bindValue(":login", login);
	if(query.exec() && query.first()
	   && query.value(0).toInt() == 0)
		return true;
	return false;
}

QString CClientRegistrationWidget::generatePassword()
{
    const static char* const dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const static int dictLen = qstrlen(dict);

    int len = 6 + qrand() % 5;
    QString pass;
    for(int i = 0; i < len; i++)
        pass.append(dict[qrand() % dictLen]);
    return pass;
}
