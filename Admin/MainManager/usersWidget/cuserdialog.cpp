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
    query.prepare("SELECT login, passwordCrypt, name FROM Users WHERE id = :id;");
    query.bindValue(":id", mId);
    if(query.exec() && query.first())
    {
        ui->leLogin->setText(query.value(0).toString());
        ui->lePassword->setText(query.value(1).toString());
        ui->leName->setText(query.value(2).toString());
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
        query.prepare("INSERT INTO Users VALUES(NULL, :login, :password, :name);");
        query.bindValue(":login", ui->leLogin->text());
        query.bindValue(":password", ui->lePassword->text());
        query.bindValue(":name", ui->leName->text());
    }
    else if(mType == Edit)
    {
        query.prepare("UPDATE Users SET passwordCrypt = :password, name = :name WHERE id = :id;");
        query.bindValue(":password", ui->lePassword->text());
        query.bindValue(":name", ui->leName->text());
        query.bindValue(":id", mId);
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
