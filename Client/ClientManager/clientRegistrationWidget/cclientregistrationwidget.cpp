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
        query.prepare("INSERT INTO Clients VALUES(NULL, :name, :birthDate, :login, :passwordHash, :clientsPhone);");
        query.bindValue(":name", ui->leFIO->text());
        query.bindValue(":birthDate", ui->cwBDate->selectedDate().toString("dd.MM.yyyy"));
        query.bindValue(":login", ui->leLogin->text());
        query.bindValue(":passwordHash", generatePassword());
        query.bindValue(":clientsPhone", ui->lePhone->text());
        query.exec();
    }

    else
    {
        QMessageBox::Yes == QMessageBox::question(this, tr("Предупреждение"), tr("Пользователь с таким логином уже существует!"), QMessageBox::Yes);
    }
}

bool CClientRegistrationWidget::validateLogin(QString login)
{
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
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

QString CClientRegistrationWidget::generatePassword()
{
    const static char* const dict = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const static int dictLen = qstrlen(dict);
    qsrand(QTime::currentTime().msec());
    int len = 6 + qrand() % 5;
    QString pass;
    for(int i = 0; i < len; i++)
        pass.append(dict[qrand() % dictLen]);
    return pass;
}
