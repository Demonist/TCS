#include "cfandialog.h"
#include "ui_cfandialog.h"

CFanDialog::CFanDialog(const QString connectionName, const int actionId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CFanDialog)
{
	ui->setupUi(this);
	mConnectionName = connectionName;
	mId = actionId;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT fanPrice, fanPenalty, fanCount FROM Actions WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->sbxPrice->setValue(query.value(0).toInt());
		ui->sbxPenalty->setValue(query.value(1).toInt());
		ui->sbxCount->setValue(query.value(2).toInt());
	}
	else
		qDebug(qPrintable(query.lastError().text()));
}

CFanDialog::~CFanDialog()
{
	delete ui;
}

void CFanDialog::on_pbnCancel_clicked()
{
	close();
}

void CFanDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("UPDATE Actions SET fanPrice = :price, fanPenalty = :penalty, fanCount = :count WHERE id = :id;");
	query.bindValue(":price", ui->sbxPrice->value());
	query.bindValue(":penalty", ui->sbxPenalty->value());
	query.bindValue(":count", ui->sbxCount->value());
	query.bindValue(":id", mId);
	if(!query.exec())
		qDebug(qPrintable(query.lastError().text()));

	accept();
	close();
}

int CFanDialog::price() const
{
	return ui->sbxPrice->value();
}

int CFanDialog::penalty() const
{
	return ui->sbxPenalty->value();
}

int CFanDialog::count() const
{
	return ui->sbxCount->value();
}
