#ifndef CACCOUNTINGWIDGET_H
#define CACCOUNTINGWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "ccategorydialog.h"
#include "cdatedialog.h"
#include "ccomplitedaction.h"
#include "global.h"
#include "ccachechecker.h"

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
	void on_twComplitedActions_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	void on_pbnExport_clicked();

private:
    Ui::CAccountingWidget *ui;
	QList<QPair<QHash<QString, int>, QHash<QString, int> > > mActionStatistics;
	QList<QPair<QHash<QString, int>, QHash<QString, int> > > mComplitedStatistics;
};

#endif // CACCOUNTINGWIDGET_H
