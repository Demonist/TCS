#ifndef CACTIONSWIDGET_H
#define CACTIONSWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "global.h"
#include "cactiondialog.h"

namespace Ui {
    class CActionsWidget;
}

class CActionsWidget : public CAbstractCommonWidget
{
    Q_OBJECT

public:
    explicit CActionsWidget(QWidget *parent = 0);
    ~CActionsWidget();

public slots:
	void updateData();

private slots:
	void on_tbnAdd_clicked();
	void on_tbnEdit_clicked();
	void on_tbnDel_clicked();

private:
    Ui::CActionsWidget *ui;
	int mCategoriesCount;
};

#endif // CACTIONSWIDGET_H
