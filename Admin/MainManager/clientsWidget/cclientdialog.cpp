#include "cclientdialog.h"
#include "ui_cclientdialog.h"

//private:

bool CClientDialog::validateLogin(QString login, Type actionType)
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(actionType == Edit)
	{
		query.prepare("SELECT COUNT(id) FROM Clients WHERE login = :login AND id <> :id;");
		query.bindValue(":id", mId);
	}
	else
		query.prepare("SELECT COUNT(id) FROM Clients WHERE login = :login;");
	query.bindValue(":login", login);
	if(query.exec() && query.first())
		return query.value(0).toInt() == 0;

	return false;
}

//public:

CClientDialog::CClientDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CClientDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление клиента"));
	mType = Add;
}

CClientDialog::CClientDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CClientDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование клиента"));
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT name, birthDate, login, passwordHash, phone FROM Clients WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leClientFIO->setText(query.value(0).toString());
		QDate dt;
		dt = QDate::fromString(query.value(1).toString(), "dd.MM.yyyy");
		ui->cwClientBirthDate->setSelectedDate(dt);
        ui->leClientLogin->setText(query.value(2).toString());
		ui->leClientPassword->setText(Global::decrypt(query.value(3).toByteArray()));
        ui->leClientPhone->setText(query.value(4).toString());
	}
}

CClientDialog::~CClientDialog()
{
	delete ui;
}

void CClientDialog::on_pbnCancel_clicked()
{
	close();
}

void CClientDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
		if(validateLogin(ui->leClientLogin->text(), Add))
		{
			query.prepare("INSERT INTO Clients VALUES(NULL, :name, :birthDate, :login, :passwordHash, :phone);");
			query.bindValue(":name", ui->leClientFIO->text());
			query.bindValue(":birthDate", ui->cwClientBirthDate->selectedDate().toString("dd.MM.yyyy"));
			query.bindValue(":login", ui->leClientLogin->text());
			query.bindValue(":passwordHash", Global::crypt(ui->leClientPassword->text()));
			query.bindValue(":phone", ui->leClientPhone->text());
			query.exec();
			emit dataWasUpdated();
			close();
		}
		else
			QMessageBox::question(this, tr("Внимание"), tr("Пользователь с таким логином уже существует!"));
	}
	else if(mType == Edit)
	{
		if(validateLogin(ui->leClientLogin->text(), Edit))
		{
			query.prepare("UPDATE Clients SET name = :name, birthDate = :birthDate, login = :login, passwordHash = :passwordHash, phone = :phone WHERE id = :id;");
			query.bindValue(":name", ui->leClientFIO->text());
			query.bindValue(":birthDate", ui->cwClientBirthDate->selectedDate().toString("dd.MM.yyyy"));
			query.bindValue(":login", ui->leClientLogin->text());
			query.bindValue(":passwordHash", Global::crypt(ui->leClientPassword->text()));
			query.bindValue(":phone", ui->leClientPhone->text());
			query.bindValue(":id", mId);
			query.exec();
			emit dataWasUpdated();
			close();
		}
		else
			QMessageBox::question(this, tr("Внимание"), tr("Пользователь с таким логином уже существует!"));
	}
}
