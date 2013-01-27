#ifndef CACTIONTICKETSMANAGEMENT_H
#define CACTIONTICKETSMANAGEMENT_H

#include <QtGui>
#include <QtSql>
#include "cactionseatitem.h"
#include "global.h"
#include "cimages.h"
#include "cgraphicsscene.h"
#include "cpricedialog.h"
#include "cabstractlegend.h"
#include "cfandialog.h"

namespace Ui {
    class CActionTicketsManagement;
}

class CActionTicketsManagement : public QDialog, public CAbstractLegend
{
    Q_OBJECT
	//types:
	enum EditType{None, Avaible, NotAvaible, Hide, Show, Price, Drag, Paint};

	//members:
protected:
	Ui::CActionTicketsManagement *ui;
	QString mConnectionName;
	int mId;		//! Идентификатор мероприятия.
	int mPlaceId;	//! Идентификатор концертной площадки, на которой будет происходить данное мероприятие.

	CGraphicsScene mScene;
	EditType mEditType;
	qreal mScale;

	//methods:
protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseSceneEvent(QMouseEvent *event);
	void updateScaleText();
	void paintLenend(QPainter *painter, const QSize &viewSize);

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
	void on_tbnPricePaint_clicked(bool checked);
	void on_tbnDrag_toggled(bool checked);
	void on_tbnPricePaint_toggled(bool checked);
	void on_tbnZoomIn_clicked();
	void on_tbnZoomOut_clicked();
	void on_tbnZoomDefault_clicked();
	void on_cbxShowBackground_toggled(bool checked);
	void on_cbxShowLegend_toggled(bool checked);
	void on_tbnPriceAdd_clicked();
	void on_tbnPriceEdit_clicked();
	void on_tbnPriceDel_clicked();
	void on_pbnFan_clicked();
};

#endif // CACTIONTICKETSMANAGEMENT_H
