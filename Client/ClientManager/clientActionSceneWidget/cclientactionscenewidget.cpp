#include "cclientactionscenewidget.h"
#include "ui_cclientactionscenewidget.h"

CClientActionSceneWidget::CClientActionSceneWidget(QWidget *parent) :
	CAbstractCommonWidget(parent),
	ui(new Ui::CClientActionSceneWidget)
{
	ui->setupUi(this);
	mScene.setDrawBounds(false);
	mScene.setBackgroundImage(QPixmap(":/clientImages/background.jpg"));
	ui->gvScene->setScene(&mScene);
}

CClientActionSceneWidget::~CClientActionSceneWidget()
{
	delete ui;
}

void CClientActionSceneWidget::updateData()
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT id FROM Actions WHERE state = :state OR state = :state2;");
	query.bindValue(":state", Global::ActionActive);
	query.bindValue(":state2", Global::ActionMoved);
	if(query.exec())
	{
		int count = 0;

		mScene.clear();
		CActionItem *item;
		while(query.next())
		{
			item = new CActionItem(mConnectionName, query.value(0).toInt());
			if(item)
			{
				item->setPos(50 + (count % 3 * 250), 50 + (count / 3 * 250));
				mScene.addItem(item);
			}
			count++;
		}
		mScene.setSceneRect(0, 0, 800, 50 + (count / 3 + 1) * 250);
	}
}
