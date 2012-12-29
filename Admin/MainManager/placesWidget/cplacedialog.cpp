#include "cplacedialog.h"
#include "ui_cplacedialog.h"

CPlaceDialog::CPlaceDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPlaceDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление площадки"));
	mType = Add;
}

CPlaceDialog::CPlaceDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPlaceDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование площадки"));
	//ui->lePlaceTitle->setEnabled(true);
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT title, address FROM Places WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->lePlaceTitle->setText(query.value(0).toString());
		ui->lePlaceAddress->setText(query.value(1).toString());
	}
}

CPlaceDialog::~CPlaceDialog()
{
	delete ui;
}

void CPlaceDialog::on_buttonBox_accepted()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
		query.prepare("INSERT INTO Places VALUES(NULL, :title, :address);");
		query.bindValue(":name", ui->lePlaceTitle->text());
		query.bindValue(":address", ui->lePlaceAddress->text());
	}
	else if(mType == Edit)
	{
		query.prepare("UPDATE Places SET title = :title, address = :address WHERE id = :id;");
		query.bindValue(":title", ui->lePlaceTitle->text());
		query.bindValue(":address", ui->lePlaceAddress->text());
		query.bindValue(":id", mId);
	}

	////TODO: :
	query.exec();

	emit dataWasUpdated();
	close();
}

void CPlaceDialog::on_buttonBox_rejected()
{
	close();
}
