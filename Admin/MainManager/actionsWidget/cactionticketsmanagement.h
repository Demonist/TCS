#ifndef CACTIONTICKETSMANAGEMENT_H
#define CACTIONTICKETSMANAGEMENT_H

#include <QtGui>
#include <QtSql>
#include "cactionseatitem.h"
#include "global.h"

namespace Ui {
    class CActionTicketsManagement;
}

class CActionTicketsManagement : public QDialog
{
    Q_OBJECT
	//types:
	enum EditType{None, Avaible, NotAvaible, Hide, Show, Price, Drag};

	//members:
protected:
	Ui::CActionTicketsManagement *ui;
	QString mConnectionName;
	int mId;		//! Идентификатор мероприятия.
	int mPlaceId;	//! Идентификатор концертной площадки, на которой будет происходить данное мероприятие.

	QGraphicsScene mScene;
	EditType mEditType;
	int mScalePersent;

	//methods:
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseSceneEvent(QMouseEvent *event);
	void updateScaleText();

public:
	explicit CActionTicketsManagement(const QString &connectionName, const int id, QWidget *parent = 0);
    ~CActionTicketsManagement();

private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();
	void on_tbnAvaible_clicked(bool checked);
	void on_tbnNotAvaible_clicked(bool checked);
	void on_tbnHide_clicked(bool checked);
	void on_tbnShow_clicked(bool checked);
	void on_tbnDrag_clicked(bool checked);
	void on_tbnDrag_toggled(bool checked);
	void on_tbnZoomIn_clicked();
	void on_tbnZoomOut_clicked();
	void on_tbnZoomDefault_clicked();
};

#endif // CACTIONTICKETSMANAGEMENT_H
