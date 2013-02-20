#include "cclientactiondialog.h"
#include "ui_cclientactiondialog.h"

bool CClientActionDialog::eventFilter(QObject *object, QEvent *event)
{
	if(event->type() == QEvent::Paint)
	{
		if(mView)
		{
			QPainter painter(ui->graphicsView->viewport());
			mView->render(&painter, mView->scene()->sceneRect().toRect(), mView->scene()->sceneRect().toRect());
			return true;
		}
	}
	return false;
}

CClientActionDialog::CClientActionDialog(QWidget *parent) :
	QDialog(parent, Qt::Window),
	ui(new Ui::CClientActionDialog)
{
	ui->setupUi(this);
	mView = 0;
	ui->graphicsView->viewport()->installEventFilter(this);
}

CClientActionDialog::~CClientActionDialog()
{
	delete ui;
}

void CClientActionDialog::on_pushButton_clicked()
{
	setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
	showMaximized();
}

void CClientActionDialog::on_pushButton_2_clicked()
{
	setWindowFlags(Qt::Window);
	showNormal();
}

void CClientActionDialog::show(CGraphicsView *view)
{
	if(view)
		mView = view;
	QDialog::show();
}
