#ifndef CCLIENTACTIONSWIDGET_H
#define CCLIENTACTIONSWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "global.h"
#include "cdatedialog.h"
#include "cgraphicsscene.h"
#include "cclientactionseatitem.h"
#include "cimages.h"
#include "cgraphicsview.h"

namespace Ui {
	class CClientActionsWidget;
}

class CClientActionsWidget : public CAbstractCommonWidget, public CAbstractLegend
{
	Q_OBJECT
	
	//types:
private:
	struct PriceGroup
	{
		QString price;
		QString name;
		QColor color;
	};

	//members:
private:
	Ui::CClientActionsWidget *ui;
	bool mCanUpdateFilter;	//! Флаг разрешающий обновить данные согласно фильтру.
	CGraphicsScene mScene;

	int mCurrentActionId;
	int mFanPriceForCurrentAction;
	int mFanCountForCurrentAction;
	QList<PriceGroup> mPriceGroupsForCurrentAction;

	QList<CClientActionSeatItem*> mSelectedSeats;

	//methods:
public:
	explicit CClientActionsWidget(QWidget *parent = 0);
	~CClientActionsWidget();
	CGraphicsView* view();
	void paintLenend(QPainter *painter, const QSize &viewSize);
	
private slots:
	void updateFilter();
	void seatSelectionChanged(CClientActionSeatItem *item, const bool isSelected);
	void on_tbnCategoryClear_clicked();
	void on_tbnPerformerCLear_clicked();
	void on_tbnPlaceClear_clicked();
	void on_tbnDate_clicked();
	void on_tbnDateClear_clicked();
	void on_tbnFilterClear_clicked();
	void on_pbnSoldTicket_clicked();
	void on_pbnBackToActions_clicked();
	void on_pbnStartSelling_clicked();
	void on_pbnBackToSeats_clicked();
	void on_pbnPrintTickets_clicked();
	void on_sbxFanSellCount_valueChanged(int arg1);

public slots:
	void updateData();
	void showActions();

signals:
	void hideLeftPanel();
	void showLeftPanel();
};

#endif // CCLIENTACTIONSWIDGET_H
