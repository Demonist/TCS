#include "cmarketdialog.h"
#include "ui_cmarketdialog.h"

CMarketDialog::CMarketDialog(const QString &connectionName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMarketDialog)
{
    ui->setupUi(this);
    mConnectionName = connectionName;
    setWindowTitle(tr("Добавление торговой площадки"));
    mType = Add;
}

CMarketDialog::CMarketDialog(const QString &connectionName, const int id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMarketDialog)
{
    ui->setupUi(this);
    mConnectionName = connectionName;
    setWindowTitle(tr("Редактирование торговой площадки"));
    mType = Edit;
    mId = id;

    QSqlQuery query(QSqlDatabase::database(mConnectionName));
    query.prepare("SELECT address FROM Markets WHERE id = :id;");
    query.bindValue(":id", mId);
    if(query.exec() && query.first())
    {
        ui->leAddress->setText(query.value(0).toString());
    }
}
CMarketDialog::~CMarketDialog()
{
    delete ui;
}

void CMarketDialog::on_pbnCancel_clicked()
{
	close();
}

void CMarketDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
			query.prepare("INSERT INTO Markets VALUES(NULL, :address);");
			query.bindValue(":address", ui->leAddress->text());
	}
	else if(mType == Edit)
	{

			query.prepare("UPDATE Markets SET address = :address WHERE id = :id;");
			query.bindValue(":address", ui->leAddress->text());
			query.bindValue(":id", mId);
	}
	query.exec();
	emit dataWasUpdated();
	close();
}
