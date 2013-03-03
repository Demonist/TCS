#include "cclientactiondialog.h"
#include "ui_cclientactiondialog.h"

void CClientActionDialog::closeEvent(QCloseEvent *event)
{
	if(mCanClose)
		event->accept();
	else
		event->ignore();
}

CClientActionDialog::CClientActionDialog(QWidget *parent) :
	QDialog(parent, Qt::Window),
	ui(new Ui::CClientActionDialog)
{
	ui->setupUi(this);
	ui->graphicsView->setDragByRightButtonEnabled(false);
	ui->graphicsView->setWheelScalling(false);
	mCanClose = true;
	QDesktopWidget *desktop = QApplication::desktop();
	if(desktop)
	{
		if(desktop->screenCount() > 1)
		{
			for(int i = 0; i < desktop->screenCount(); i++)
				if(i != desktop->primaryScreen())	//Находим второстепенный экран
				{
					QRect secondaryScreenGeometry = desktop->screenGeometry(i);
					move(secondaryScreenGeometry.topLeft());
					resize(secondaryScreenGeometry.size());
					setWindowFlags(Qt::Window|Qt::FramelessWindowHint);
					showMaximized();
					showFullScreen();
					mCanClose = false;
					break;
				}
		}
	}
}

CClientActionDialog::~CClientActionDialog()
{
	delete ui;
}

void CClientActionDialog::setSourceView(CSourceGraphicsView *sourceView)
{
	if(sourceView)
	{
		ui->graphicsView->setScene(sourceView->scene());
		ui->graphicsView->setLegend(sourceView->legend());
		connect(sourceView, SIGNAL(scaleChanged(qreal)), this, SLOT(setScale(qreal)));
		connect(sourceView, SIGNAL(dragChanged(int,int)), this, SLOT(drag(int,int)));
	}
}

void CClientActionDialog::setScale(const qreal scale)
{
	ui->graphicsView->setScale(scale);
}

void CClientActionDialog::drag(int x, int y)
{
	if(ui->graphicsView->horizontalScrollBar())
		ui->graphicsView->horizontalScrollBar()->setValue(x);
	if(ui->graphicsView->verticalScrollBar())
		ui->graphicsView->verticalScrollBar()->setValue(y);
}
