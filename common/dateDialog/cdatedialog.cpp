#include "cdatedialog.h"
#include "ui_cdatedialog.h"

CDateDialog::CDateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDateDialog)
{
    ui->setupUi(this);
	mSelectedDate = QDate::currentDate();
	ui->cDate->setSelectedDate(mSelectedDate);
}

CDateDialog::CDateDialog(const QDate &date, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CDateDialog)
{
	ui->setupUi(this);
	mSelectedDate = date;
	ui->cDate->setSelectedDate(mSelectedDate);
}

CDateDialog::~CDateDialog()
{
    delete ui;
}

QDate CDateDialog::selectedDate() const
{
	return mSelectedDate;
}

void CDateDialog::on_pbnCancel_clicked()
{
	reject();
}

void CDateDialog::on_pbnOk_clicked()
{
	mSelectedDate = ui->cDate->selectedDate();
	accept();
}

void CDateDialog::setDate(const QDate &date)
{
	mSelectedDate = date;
	ui->cDate->setSelectedDate(mSelectedDate);
}
