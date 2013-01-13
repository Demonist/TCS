#include "cuserdialog.h"
#include "ui_cuserdialog.h"

CUserDialog::CUserDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление пользователя"));
	mType = Add;
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.exec("SELECT id, address FROM Markets");
    int incrementIndex = 0;
    while(query.next())
    {
        ui->cbxMarkets->addItem(query.value(1).toString());
        ui->cbxMarkets->setItemData(incrementIndex, query.value(0).toString());
        incrementIndex++;
    }


}

CUserDialog::CUserDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование пользователя"));
	ui->leLogin->setEnabled(false); //Запрещаем изменение логина.
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));



    query.prepare("SELECT login, passwordCrypt, name, address, Markets.ID FROM Users, Markets WHERE Users.id = :id AND marketsID = Markets.ID;");
	query.bindValue(":id", mId);    
	if(query.exec() && query.first())
	{
		ui->leLogin->setText(query.value(0).toString());
		ui->lePassword->setText(query.value(1).toString());
		ui->leName->setText(query.value(2).toString());
        ui->cbxMarkets->addItem(query.value(3).toString());
        ui->cbxMarkets->setItemData(ui->cbxMarkets->currentIndex(), query.value(4).toString());
    }
    query.exec("SELECT id, address FROM Markets WHERE id <> (SELECT marketsid FROM Users WHERE id = " + QString::number(mId) + ");");
    int cIndex = 1;
    while(query.next())
    {
        ui->cbxMarkets->addItem(query.value(1).toString());
        ui->cbxMarkets->setItemData(cIndex, query.value(0).toString());
        cIndex++;
    }

}

CUserDialog::~CUserDialog()
{
	delete ui;
}

void CUserDialog::on_buttonBox_accepted()
{
	////TODO: Добавить проверку входных данных: поля логина/пароля не должны быть пустыми; логин не должен повторяться.

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
        query.prepare("INSERT INTO Users VALUES(NULL, :login, :password, :name, :marketsID);");
		query.bindValue(":login", ui->leLogin->text());
		query.bindValue(":password", ui->lePassword->text());
		query.bindValue(":name", ui->leName->text());
        query.bindValue(":marketsID", ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toInt());
        qDebug(ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toByteArray());
	}
	else if(mType == Edit)
	{
        query.prepare("UPDATE Users SET passwordCrypt = :password, name = :name, marketsID = :marketsID WHERE id = :id;");
		query.bindValue(":password", ui->lePassword->text());
		query.bindValue(":name", ui->leName->text());
        query.bindValue(":marketsID", ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toInt());
		query.bindValue(":id", mId);
        qDebug(ui->cbxMarkets->itemData(ui->cbxMarkets->currentIndex()).toByteArray());
	}

	////TODO: Добавить обработчик ошибок:
	query.exec();

	emit dataWasUpdated();
	close();
}

void CUserDialog::on_buttonBox_rejected()
{
	close();
}
