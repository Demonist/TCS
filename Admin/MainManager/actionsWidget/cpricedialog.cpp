#include "cpricedialog.h"
#include "ui_cpricedialog.h"

//private;

void CPriceDialog::setColor(const QString &color)
{
	ui->leColor->setStyleSheet("QLineEdit{background-color: " + color + "};");
}

//public:

/**
Конструктор добавления.
\param actionId обязателен в отличии от конструктора редактирования.
*/
CPriceDialog::CPriceDialog(const QString &connectionName, const int actionId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPriceDialog)
{
	ui->setupUi(this);
	setWindowTitle(tr("Добавление ценовой группы"));
	mType = Add;
	mConnectionName = connectionName;
	mActionId = actionId;
}

/**
Конструктор редактирования.
\param actionId Передавать не обязательно, он нужен чтобы различать контрукторы.
*/
CPriceDialog::CPriceDialog(const QString &connectionName, const int actionId, const int priceGroupId, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CPriceDialog)
{
	ui->setupUi(this);
	setWindowTitle(tr("Редактирование ценовой группы"));
	mType = Edit;
	mConnectionName = connectionName;
	mId = priceGroupId;
	mActionId = actionId;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT name, price, penalty, color FROM ActionPriceGroups WHERE id = :id;");
	query.bindValue(":id", mId);
	if(query.exec() && query.first())
	{
		ui->leName->setText(query.value(0).toString());
		ui->sbxPrice->setValue(query.value(1).toInt());
		ui->sbxPenalty->setValue(query.value(2).toInt());
		mColor = query.value(3).toString();
		setColor(mColor);
	}
}

CPriceDialog::~CPriceDialog()
{
	delete ui;
}

QString CPriceDialog::name() const
{
	return ui->leName->text();
}

int CPriceDialog::price() const
{
	return ui->sbxPrice->value();
}

int CPriceDialog::penalty() const
{
	return ui->sbxPenalty->value();
}

void CPriceDialog::on_pbnCancel_clicked()
{
	close();
}

void CPriceDialog::on_pbnApply_clicked()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));

	if(mType == Add)
	{
		query.prepare("INSERT INTO ActionPriceGroups VALUES(NULL, :actId, :name, :price, :penalty, :color);");
		query.bindValue(":actId", mActionId);
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":price", ui->sbxPrice->value());
		query.bindValue(":penalty", ui->sbxPenalty->value());
		query.bindValue(":color", mColor);
	}
	else if(mType == Edit)
	{
		query.prepare("UPDATE ActionPriceGroups SET name = :name, price = :price, penalty = :penalty, color = :color WHERE id = :id;");
		query.bindValue(":id", mId);
		query.bindValue(":name", ui->leName->text());
		query.bindValue(":price", ui->sbxPrice->value());
		query.bindValue(":penalty", ui->sbxPenalty->value());
		query.bindValue(":color", mColor);
	}

	if(query.exec())
	{
		if(mType == Add
		   && query.exec("SELECT MAX(id) FROM ActionPriceGroups;") && query.first())
			mId = query.value(0).toInt();
	}
	else
		qDebug(qPrintable("CPriceDialog::on_pbnApply_clicked: sql error - " + query.lastError().text()));

	accept();
	close();
}

void CPriceDialog::on_tbnColor_clicked()
{
	QColor color(mColor);
	if(mType == Add)
		color = qRgb(qrand()%255, qrand()%255, qrand()%255);
	color = QColorDialog::getColor(color, this, tr("Выберите цвет ценовой группы"));
	if(color.isValid())
	{
		mColor = color.name();
		setColor(mColor);
	}
}
