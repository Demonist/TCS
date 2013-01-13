#include "cclientdialog.h"
#include "ui_cclientdialog.h"

CClientDialog::CClientDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CClientDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление категории"));
	mType = Add;
}

CClientDialog::CClientDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CClientDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование категории"));
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT name, birthDate, login, passwordHash FROM Clients WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leClientFIO->setText(query.value(0).toString());
		QDate dt;
		dt = QDate::fromString(query.value(1).toString(), "dd.MM.yyyy");
		ui->cwClientBirthDate->setSelectedDate(dt);
        ui->leClientLogin->setText(query.value(2).toString());
        ui->leClientPassword->setText(query.value(3).toString());
	}
}

CClientDialog::~CClientDialog()
{
	delete ui;
}

void CClientDialog::on_buttonBox_accepted()
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
        if(validateLogin(ui->leClientLogin->text(), Add))
        {
            query.prepare("INSERT INTO Clients VALUES(NULL, :name, :birthDate, :login, :passwordHash);");
            query.bindValue(":name", ui->leClientFIO->text());
            query.bindValue(":birthDate", ui->cwClientBirthDate->selectedDate().toString("dd.MM.yyyy"));
            query.bindValue(":login", ui->leClientLogin->text());
            query.bindValue(":passwordHash", ui->leClientPassword->text());
            query.exec();
            emit dataWasUpdated();
            close();
        }
        else
        {
            QMessageBox::Yes == QMessageBox::question(this, tr("Предупреждение"), tr("Пользователь с таким логином уже существует!"), QMessageBox::Yes);
        }

	}
	else if(mType == Edit)
	{
        if(validateLogin(ui->leClientLogin->text(), Edit, mId))
        {
            query.prepare("UPDATE Clients SET name = :name, birthDate = :birthDate, login = :login, passwordHash = :passwordHash WHERE id = :id;");
            query.bindValue(":name", ui->leClientFIO->text());
            query.bindValue(":birthDate", ui->cwClientBirthDate->selectedDate().toString("dd.MM.yyyy"));
            query.bindValue(":login", ui->leClientLogin->text());
            query.bindValue(":passwordHash", ui->leClientPassword->text());
            query.bindValue(":id", mId);
            query.exec();
            emit dataWasUpdated();
            close();
        }
        else
        {
            QMessageBox::Yes == QMessageBox::question(this, tr("Предупреждение"), tr("Пользователь с таким логином уже существует!"), QMessageBox::Yes);
        }
	}
}

void CClientDialog::on_buttonBox_rejected()
{
	close();
}



bool CClientDialog::validateLogin(QString login, Type actionType, int id)
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    if(actionType == Edit)
    {
        if(query.exec("SELECT COUNT(id) AS id FROM Clients WHERE login = '" + login + "' AND id <> '"+ QString::number(id) +"';"))
        {
            while(query.next())
            {
                if(query.value(0).toInt() == 0)
                    return true;
                return false;
            }
        }
    }
    else
    {
        if(query.exec("SELECT COUNT(id) AS id FROM Clients WHERE login = '" + login + "';"))
        {
            while(query.next())
            {
                if(query.value(0).toInt() == 0)
                    return true;
                return false;
            }
        }
    }
}
