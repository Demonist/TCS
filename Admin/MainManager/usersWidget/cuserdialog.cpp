#include "cuserdialog.h"
#include "ui_cuserdialog.h"

CUserDialog::CUserDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление кассира"));
	mType = Add;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT id, address FROM Markets"))
		while(query.next())
			ui->cbxMarkets->addItem(query.value(1).toString(), query.value(0).toString());
}

CUserDialog::CUserDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование кассира"));
	ui->leLogin->setEnabled(false); //Запрещаем изменение логина.
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	query.prepare("SELECT Users.login, Users.passwordCrypt, Users.name, Markets.address, Markets.id FROM Users, Markets WHERE Users.id = :id AND Users.id_market = Markets.id;");
	query.bindValue(":id", mId);    
	if(query.exec() && query.first())
	{
		ui->leLogin->setText(query.value(0).toString());
		ui->lePassword->setText(Global::decrypt(query.value(1).toByteArray()));
		ui->leName->setText(query.value(2).toString());
		ui->cbxMarkets->addItem(query.value(3).toString(), query.value(4));
    }
	query.prepare("SELECT Markets.id, Markets.address FROM Markets, Users WHERE Markets.id <> Users.id_market AND Users.id = :id;");
	query.bindValue(":id", mId);
	if(query.exec())
		while(query.next())
			ui->cbxMarkets->addItem(query.value(1).toString(), query.value(0));
	else
		qDebug(qPrintable(query.lastError().text()));
}

CUserDialog::~CUserDialog()
{
	delete ui;
}

void CUserDialog::on_buttonBox_accepted()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	//Проверка входных данных:
	QString error;
	if(ui->leLogin->text().isEmpty())
		error += tr("Поле логина не должно быть пустым.\n");
	else if(mType == Add)
	{
		query.prepare("SELECT COUNT(id) FROM Users WHERE login = :login;");
		query.bindValue(":login", ui->leLogin->text());
		if(query.exec() && query.first() && query.value(0).toInt() != 0)
			error += tr("Указанный логин уже занят.\n");
	}
	if(ui->lePassword->text().isEmpty())
		error += tr("Поле пароля не должно быть пустым.\n");

	if(error.isEmpty() == false)
	{
		QMessageBox::warning(this, tr("Внимание"), error);
		return;
	}

	if(mType == Add)
	{
		query.prepare("INSERT INTO Users VALUES(NULL, :login, :password, :name, :marketsID);");
		query.bindValue(":login", ui->leLogin->text());
		query.bindValue(":password", Global::crypt(ui->lePassword->text()));
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":marketsID", ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toInt());
	}
	else if(mType == Edit)
	{
		query.prepare("UPDATE Users SET passwordCrypt = :password, name = :name, id_market = :marketsID WHERE id = :id;");
		query.bindValue(":password", Global::crypt(ui->lePassword->text()));
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":marketsID", ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toInt());
		query.bindValue(":id", mId);
	}

	if(!query.exec())
		qDebug(qPrintable(query.lastError().text()));

	emit dataWasUpdated();
	close();
}

void CUserDialog::on_buttonBox_rejected()
{
	close();
}
