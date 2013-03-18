#include "ccontrolupdatedbase.h"
#include "ui_ccontrolupdatedbase.h"

CControlUpdateDBase::CControlUpdateDBase(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CControlUpdateDBase)
{
	ui->setupUi(this);
}

CControlUpdateDBase::~CControlUpdateDBase()
{
	delete ui;
}
