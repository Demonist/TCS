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
        query.prepare("INSERT INTO Clients VALUES(NULL, :name, :birthDate, :login, :passwordHash);");
        query.bindValue(":name", ui->leClientFIO->text());
        query.bindValue(":birthDate", ui->cwClientBirthDate->selectedDate().toString("dd.MM.yyyy"));
        query.bindValue(":login", ui->cbClientLogin->currentText());
        query.bindValue(":passwordHash", ui->leClientPassword->text());
    }
    else if(mType == Edit)
    {
        /*/query.prepare("UPDATE Categories SET name = :name WHERE id = :id;");
        query.bindValue(":name", ui->leName->text());
        query.bindValue(":id", mId);*/
    }

    ////TODO: :
    query.exec();
    emit dataWasUpdated();
    close();
}

void CClientDialog::on_buttonBox_destroyed()
{
    close();
}

QList<QString> CClientDialog::generateLogins(QString name, QString birthDate)
{
    QMap<QString, QString> map;
    map[tr("а")] = "a";
    map[tr("б")] = "b";
    map[tr("в")] = "v";
    map[tr("г")] = "g";
    map[tr("д")] = "d";
    map[tr("е")] = "e";
    map[tr("ё")] = "jo";
    map[tr("ж")] = "zh";
    map[tr("з")] = "z";
    map[tr("и")] = "i";
    map[tr("й")] = "j";
    map[tr("к")] = "k";
    map[tr("л")] = "l";
    map[tr("м")] = "m";
    map[tr("н")] = "n";
    map[tr("о")] = "o";
    map[tr("п")] = "p";
    map[tr("р")] = "r";
    map[tr("с")] = "s";
    map[tr("т")] = "t";
    map[tr("у")] = "u";
    map[tr("ф")] = "f";
    map[tr("х")] = "h";
    map[tr("ц")] = "c";
    map[tr("ч")] = "ch";
    map[tr("ш")] = "sh";
    map[tr("щ")] = "shh";
    map[tr("ъ")] = "";
    map[tr("ы")] = "y";
    map[tr("ь")] = "";
    map[tr("э")] = "je";
    map[tr("ю")] = "ju";
    map[tr("я")] = "a";
    map[tr("А")] = "A";
    map[tr("Б")] = "B";
    map[tr("В")] = "V";
    map[tr("Г")] = "G";
    map[tr("Д")] = "D";
    map[tr("Е")] = "E";
    map[tr("Ё")] = "Jo";
    map[tr("Ж")] = "Zh";
    map[tr("З")] = "Z";
    map[tr("И")] = "I";
    map[tr("Й")] = "J";
    map[tr("К")] = "K";
    map[tr("Л")] = "L";
    map[tr("М")] = "M";
    map[tr("Н")] = "N";
    map[tr("О")] = "O";
    map[tr("П")] = "P";
    map[tr("Р")] = "R";
    map[tr("С")] = "S";
    map[tr("Т")] = "T";
    map[tr("У")] = "U";
    map[tr("Ф")] = "F";
    map[tr("Х")] = "H";
    map[tr("Ц")] = "C";
    map[tr("Ч")] = "Ch";
    map[tr("Ш")] = "Sh";
    map[tr("Щ")] = "Shh";
    map[tr("Ъ")] = "";
    map[tr("Ы")] = "Y";
    map[tr("Ь")] = "";
    map[tr("Э")] = "Je";
    map[tr("Ю")] = "Ju";
    map[tr("Я")] = "Ja";
    map["0"] = "0";
    map["1"] = "1";
    map["2"] = "2";
    map["3"] = "3";
    map["4"] = "4";
    map["5"] = "5";
    map["6"] = "6";
    map["7"] = "7";
    map["8"] = "8";
    map["9"] = "9";
    map[" "] = " ";

    QList<QString> logins;
    ui->cbClientLogin->clear();
    QString s = ui->leClientFIO->text();
    QString ns = "";
    int j = s.length();
    for(int i = 0; i < j; i++)
    {
        ns += map.value(""+ s[i] +"");
    }
    logins << ns;
    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.prepare("SELECT name FROM Clients like '%"+ ns +"%';");
    query.bindValue(":id", mId);
    if(query.exec() && query.first())
    {
        ui->leClientFIO->setText(query.value(0).toString());
        //ui->cwClientBirthDate->setDateTextFormat();
    }

}

QString CClientDialog::validateLogin(QString login)
{
    /*QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.prepare("SELECT name FROM Clients like '%"+ ns +"%';");
    query.bindValue(":id", mId);
    if(query.exec() && query.first())
    {
        ui->leClientFIO->setText(query.value(0).toString());
        //ui->cwClientBirthDate->setDateTextFormat();
    }*/
}
