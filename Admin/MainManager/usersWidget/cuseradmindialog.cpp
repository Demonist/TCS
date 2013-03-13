#include "cuseradmindialog.h"
#include "ui_cuseradmindialog.h"

CUserAdminDialog::CUserAdminDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserAdminDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление адмнистратора"));
	mType = Add;
}

CUserAdminDialog::CUserAdminDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CUserAdminDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование адмнистратора"));
	ui->leLogin->setEnabled(false); //Запрещаем изменение логина.
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT login, passwordCrypt, name FROM Admins WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leLogin->setText(query.value(0).toString());
		ui->lePassword->setText(Global::decrypt(query.value(1).toByteArray()));
		ui->leName->setText(query.value(2).toString());
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}

CUserAdminDialog::~CUserAdminDialog()
{
	delete ui;
}

void CUserAdminDialog::on_pbnCancel_clicked()
{
	close();
}

void CUserAdminDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	//Проверка входных данных:
	QString error;
	if(ui->leLogin->text().isEmpty())
		error += tr("Поле логина не должно быть пустым.\n");
	else if(mType == Add)
	{
		query.prepare("SELECT COUNT(id) FROM Admins WHERE login = :login;");
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
		query.prepare("INSERT INTO Admins VALUES(NULL, :login, :password, :name);");
		query.bindValue(":login", ui->leLogin->text());
		query.bindValue(":password", Global::crypt(ui->lePassword->text()));
		query.bindValue(":name", ui->leName->text());
	}
	else if(mType == Edit)
	{
		query.prepare("UPDATE Admins SET passwordCrypt = :password, name = :name WHERE id = :id;");
		query.bindValue(":password", Global::crypt(ui->lePassword->text()));
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":id", mId);
	}

	if(!query.exec())
		qDebug(qPrintable(query.lastError().text()));

	emit dataWasUpdated();
	close();
}
