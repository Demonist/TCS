#include "cclientactiondialog.h"
#include "ui_cclientactiondialog.h"

//protected:

void CClientActionDialog::closeEvent(QCloseEvent *event)
{
	if(mCanClose)
		event->accept();
	else
		event->ignore();
}

//public:

CClientActionDialog::CClientActionDialog(QWidget *parent) :
	QDialog(parent, Qt::Window),
	ui(new Ui::CClientActionDialog)
{
	ui->setupUi(this);
	ui->stackedWidget->setCurrentIndex(1);
	ui->gvSeats->setDragByRightButtonEnabled(false);
	ui->gvSeats->setWheelScalling(false);
	mCanClose = true;
	mOnSecondScreen = false;

	mAdvertisementTimer.setSingleShot(false);
	mAdvertisementTimer.setInterval(10000);
	connect(&mAdvertisementTimer, SIGNAL(timeout()), this, SLOT(changeItem()));
	mAdvertisementTimer.start();

	mAdvertisementItemCurrentIndex = -1;
	mAdvertisementScene.setBackgroundBrush(QColor(30, 30, 30));
	mAdvertisementScene.setSceneRect(-400.0f, -300.0f, 800.0f, 600.0f);
	ui->gvAdvertisement->setScene(&mAdvertisementScene);

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
					QRectF sceneRect = static_cast<QRectF>(secondaryScreenGeometry);
					sceneRect.moveCenter(QPointF(0.0f, 0.0f));
					mAdvertisementScene.setSceneRect(sceneRect);

					setWindowFlags(Qt::Window|Qt::FramelessWindowHint);

					mCanClose = false;
					mOnSecondScreen = true;
					break;
				}
		}
	}
}

CClientActionDialog::~CClientActionDialog()
{
	mAdvertisementTimer.stop();
	delete ui;
}

void CClientActionDialog::setSourceView(CSourceGraphicsView *sourceView)
{
	if(sourceView)
	{
		ui->gvSeats->setScene(sourceView->scene());
		ui->gvSeats->setLegend(sourceView->legend());
		connect(sourceView, SIGNAL(scaleChanged(qreal)), this, SLOT(setScale(qreal)));
		connect(sourceView, SIGNAL(dragChanged(int,int)), this, SLOT(drag(int,int)));
	}
}

void CClientActionDialog::changeItem()
{
	mAdvertisementMutex.lock();

	if(mAdvertisementItems.size() > 1)
	{
		if(mAdvertisementItemCurrentIndex >= 0)
			mAdvertisementItems[mAdvertisementItemCurrentIndex]->setOpacityAnimated(0.0f, 2000);

		int newIndex;
		do
		{
			newIndex = qrand() % mAdvertisementItems.size();
		}while(newIndex == mAdvertisementItemCurrentIndex);
		mAdvertisementItemCurrentIndex = newIndex;
		mAdvertisementItems[mAdvertisementItemCurrentIndex]->setOpacityAnimated(1.0f, 2000);
	}

	mAdvertisementMutex.unlock();
}

void CClientActionDialog::setScale(const qreal scale)
{
	ui->gvSeats->setScale(scale);
}

void CClientActionDialog::drag(int x, int y)
{
	if(ui->gvSeats->horizontalScrollBar())
		ui->gvSeats->horizontalScrollBar()->setValue(x);
	if(ui->gvSeats->verticalScrollBar())
		ui->gvSeats->verticalScrollBar()->setValue(y);
}

void CClientActionDialog::updateAdvertisements(const QString &connectionName)
{
	static CCacheChecker cacheChecker("Advertisements", connectionName);
	if(cacheChecker.isNeedUpdate())
	{
		cacheChecker.setUpdated();

		QSqlQuery query(QSqlDatabase::database(connectionName));
		if(query.exec("SELECT id FROM Advertisements;"))
		{
			mAdvertisementMutex.lock();

			mAdvertisementItems.clear();
			mAdvertisementScene.clear();
			mAdvertisementItemCurrentIndex = -1;

			CAdvertisementItem *item;
			while(query.next())
			{
				item = new CAdvertisementItem;
				if(item)
				{
					item->setId(query.value(0).toInt());
					item->setPixmap( CImages::instance()->image(item->id()) );
					mAdvertisementItems.append(item);
					item->setOpacity(0.0f);
					mAdvertisementScene.addItem(item);
				}
			}

			mAdvertisementMutex.unlock();

			changeItem();
		}
	}
}

void CClientActionDialog::show()
{
	if(mOnSecondScreen)
	{
		showMaximized();
		showFullScreen();
	}
	else
	{
		showMinimized();
	}
}

void CClientActionDialog::showSeats()
{
	mAdvertisementTimer.stop();
	ui->stackedWidget->setCurrentIndex(0);
	if(mAdvertisementItemCurrentIndex >= 0)
	{
		mAdvertisementItems[mAdvertisementItemCurrentIndex]->setOpacity(0.0f);
		mAdvertisementItemCurrentIndex = -1;
	}
}

void CClientActionDialog::showAdvertisements()
{
	mAdvertisementTimer.start();
	ui->stackedWidget->setCurrentIndex(1);
	changeItem();
}
