#ifndef CCLIENTACTIONSCENEWIDGET_H
#define CCLIENTACTIONSCENEWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "cgraphicsscene.h"
#include "global.h"
#include "cactionitem.h"

namespace Ui {
	class CClientActionSceneWidget;
}

class CClientActionSceneWidget : public CAbstractCommonWidget
{
	Q_OBJECT
	
public:
	explicit CClientActionSceneWidget(QWidget *parent = 0);
	~CClientActionSceneWidget();
	
public slots:
	void updateData();

private:
	Ui::CClientActionSceneWidget *ui;
	CGraphicsScene mScene;
};

#endif // CCLIENTACTIONSCENEWIDGET_H
