#include "cclientregistrationwidget.h"
#include "ui_cclientregistrationwidget.h"

CClientRegistrationWidget::CClientRegistrationWidget(QWidget *parent) :
    CAbstractCommonWidget(parent),
    ui(new Ui::CClientRegistrationWidget)
{
    ui->setupUi(this);
    generatePassword("dgh56uyjmyjm");
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
        query.bindValue(":birthDate", ui->leBirthDate->text());
        query.bindValue(":login", ui->leLogin->text());
        //query.bindValue(":passwordHash", generatePassword());
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

QString CClientRegistrationWidget::generatePassword(QString hs)
{
    QList<QString> charsTable;
    charsTable << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M"
               << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z"
               << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << "k" << "l" << "m"
               << "n" << "o" << "p" << "q" << "r" << "s" << "t" << "u" << "v" << "w" << "x" << "y" << "z"
               << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0";

    int passwordLenght = qrand();

   // QByteArray md5Hash = QCryptographicHash::hash("rfgnfgm", QCryptographicHash::Md5);



    QString s = QString::number(passwordLenght);
    qDebug(qPrintable(s));
    return s;

}

void CClientRegistrationWidget::on_tbnBirthDate_clicked()
{
    CDateDialog dateDialog(this);
    if(ui->leBirthDate->text().isEmpty() == false)
        dateDialog.setDate(QDate::fromString(ui->leBirthDate->text(), "dd.MM.yyyy"));
    dateDialog.exec();
    if(dateDialog.result())
    {
        ui->leBirthDate->setText(dateDialog.selectedDate().toString("dd.MM.yyyy"));
    }
}
