#ifndef CACCOUNTINGWIDGET_H
#define CACCOUNTINGWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "ccategorydialog.h"
#include "cdatedialog.h"
#include "ccomplitedaction.h"
#include "global.h"

namespace Ui {
class CAccountingWidget;
}

class CAccountingWidget : public CAbstractCommonWidget
{
    Q_OBJECT
    
public:
    explicit CAccountingWidget(QWidget *parent = 0);
    ~CAccountingWidget();

public slots:
    void updateData();

private slots:
	void on_cbxActions_currentIndexChanged(int index);
	void on_twActions_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::CAccountingWidget *ui;
    void sumPriceTicketsForAction();
	QList<QPair<QHash<QString, int>, QHash<QString, int> > > mActionStatistics;
};

#endif // CACCOUNTINGWIDGET_H
