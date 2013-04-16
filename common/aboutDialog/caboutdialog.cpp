#include "caboutdialog.h"
#include "ui_caboutdialog.h"

CAboutDialog::CAboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CAboutDialog)
{
	ui->setupUi(this);
	setWindowIcon(QIcon(":/images/about.png"));
	setFixedSize(size());
	setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
}

CAboutDialog::~CAboutDialog()
{
	delete ui;
}

void CAboutDialog::on_pushButton_clicked()
{
	close();
}
