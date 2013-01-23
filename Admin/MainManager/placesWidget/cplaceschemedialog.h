#ifndef CPLACESCHEMEDIALOG_H
#define CPLACESCHEMEDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cseatitem.h"
#include "cdistanceitem.h"
#include "cimagedialog.h"
#include "cgraphicsscene.h"

namespace Ui {
class CPlaceSchemeDialog;
}

class CPlaceSchemeDialog : public QDialog
{
	Q_OBJECT
	
private:
	enum EditType{None, Select, Add, Move, Delete, Drag};

private:
	Ui::CPlaceSchemeDialog *ui;
	QString mConnectionName;
	int mId;	//! Идентификатор концертной площадки.
	QList<int> mDeletedIds;

	qreal mScale;
	int mSeatsCount;

	CGraphicsScene mScene;
	EditType mEditType;
	CSeatItem *mAddItem;
	CSeatItem *mSelectedItem;

	CDistanceItem *mDistanceLeftItem;
	CDistanceItem *mDistanceUpItem;
	CDistanceItem *mDistanceRightItem;
	CDistanceItem *mDistanceDownItem;
	CDistanceItem *mDistanceLeftLeftItem;
	CDistanceItem *mDistanceUpUpItem;
	CDistanceItem *mDistanceRightRightItem;
	CDistanceItem *mDistanceDownDownItem;

private:
	void drawDistances(const QGraphicsItem &item);
	void hideDistances();
	QPoint toGrid(const QPoint &pos);
	inline QPoint toGrid(const QPointF &pos);

	bool eventFilter(QObject *obj, QEvent *event);
	void updateSeatsCountText();
	void updateScaleText();

public:
	explicit CPlaceSchemeDialog(const QString &connectionName, const int placeId, QWidget *parent = 0);
	~CPlaceSchemeDialog();
	
private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();
	void on_tbnZoomIn_clicked();
	void on_tbnZoomOut_clicked();
	void on_tbnZoomDefault_clicked();
	void on_tbnSelect_clicked(bool checked);
	void on_tbnMove_clicked(bool checked);
	void on_tbnAdd_clicked(bool checked);
	void on_tbnDel_clicked(bool checked);
	void on_tbnDrag_clicked(bool checked);
	void on_leSeatNumber_textEdited(const QString &arg1);
	void on_leRow_textEdited(const QString &arg1);
	void on_tbnSelect_toggled(bool checked);
	void on_tbnDrag_toggled(bool checked);
	void on_pbnBackground_clicked();
	void on_chbxShowBackground_toggled(bool checked);
	void on_chbxShowAxis_toggled(bool checked);
	void on_sbxWidth_valueChanged(int arg1);
	void on_sbxHeight_valueChanged(int arg1);
};

//inline implementation:

inline QPoint CPlaceSchemeDialog::toGrid(const QPointF &pos)
{
	return toGrid(pos.toPoint());
}

#endif // CPLACESCHEMEDIALOG_H
