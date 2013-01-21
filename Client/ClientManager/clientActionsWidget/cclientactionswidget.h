#ifndef CCLIENTACTIONSWIDGET_H
#define CCLIENTACTIONSWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "global.h"
#include "cdatedialog.h"

namespace Ui {
	class CClientActionsWidget;
}

class CClientActionsWidget : public CAbstractCommonWidget
{
	Q_OBJECT
	
private:
	Ui::CClientActionsWidget *ui;
	bool mCanUpdateFilter;

public:
	explicit CClientActionsWidget(QWidget *parent = 0);
	~CClientActionsWidget();
	
private slots:
	void updateFilter();
	void on_tbnCategoryClear_clicked();
	void on_tbnPerformerCLear_clicked();
	void on_tbnPlaceClear_clicked();
	void on_tbnDate_clicked();
	void on_tbnDateClear_clicked();
	void on_tbnFilterClear_clicked();
	void on_pbnUpdate_clicked();

public slots:
	void updateData();
};

#endif // CCLIENTACTIONSWIDGET_H
