#include "cactionswidget.h"
#include "ui_cactionswidget.h"

#define ID 0
#define TITLE 1
#define PERFORMER 3
#define DATETIME 4
#define PLACE 5
#define CATEGORY 6

CActionsWidget::CActionsWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CActionsWidget)
{
	ui->setupUi(this);
	ui->twActions->hideColumn(ID);
	ui->twActions->setColumnWidth(ID, 0);
}

CActionsWidget::~CActionsWidget()
{
	delete ui;
}
