#include "ccategorydialog.h"
#include "ui_ccategorydialog.h"

CCategoryDialog::CCategoryDialog(const QString &connectionName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CCategoryDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Добавление категории"));
	mType = Add;
}

CCategoryDialog::CCategoryDialog(const QString &connectionName, const int id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CCategoryDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	setWindowTitle(tr("Редактирование категории"));
	ui->leName->setEnabled(true);
	mType = Edit;
	mId = id;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT name FROM Categories WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leName->setText(query.value(0).toString());
	}
}

CCategoryDialog::~CCategoryDialog()
{
	delete ui;
}


void CCategoryDialog::on_buttonBox_accepted()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(mType == Add)
	{
		query.prepare("INSERT INTO Categories VALUES(NULL, :name);");
		query.bindValue(":name", ui->leName->text());
	}
	else if(mType == Edit)
	{
		query.prepare("UPDATE Categories SET name = :name WHERE id = :id;");
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":id", mId);
	}

	////TODO: :
	query.exec();

	emit dataWasUpdated();
	close();
}

void CCategoryDialog::on_buttonBox_rejected()
{
	close();
}
