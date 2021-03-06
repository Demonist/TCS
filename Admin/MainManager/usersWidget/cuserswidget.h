#ifndef CUSERSWIDGET_H
#define CUSERSWIDGET_H

#include <QtGui>
#include "cuserdialog.h"
#include "cabstractcommonwidget.h"
#include "global.h"
#include "cuseradmindialog.h"

namespace Ui {
class CUsersWidget;
}

class CUsersWidget : public CAbstractCommonWidget
{
	Q_OBJECT
private:
	Ui::CUsersWidget *ui;
	int mMarketsCount;

public:
	explicit CUsersWidget(QWidget *parent = 0);
	~CUsersWidget();

public slots:
	void updateData();

private slots:
	void on_tbnAdd_clicked();
	void on_tbnEdit_clicked();
	void on_tbnDel_clicked();
	void on_tbnAdminAdd_clicked();
	void on_tbnAdminEdit_clicked();
	void on_tbnAdminDel_clicked();
};

#endif // CUSERSWIDGET_H
