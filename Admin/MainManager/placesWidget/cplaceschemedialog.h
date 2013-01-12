#ifndef CPLACESCHEMEDIALOG_H
#define CPLACESCHEMEDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cseatitem.h"

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
	int mId;
	QList<int> mDeletedIds;

	int mScalePersent;
	int mSeatsCount;

	QGraphicsScene mScene;
	EditType mEditType;
	CSeatItem *mAddItem;
	CSeatItem *mSelectedItem;

private:
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
	void on_tbnSelect_toggled(bool checked);
	void on_tbnDrag_toggled(bool checked);
};

#endif // CPLACESCHEMEDIALOG_H
