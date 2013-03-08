#ifndef CACCOUNTINGWIDGET_H
#define CACCOUNTINGWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "ccategorydialog.h"
#include "caccountingactionsdialog.h"
#include "cdatedialog.h"
#include "cstatistics.h"

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

    void on_cbxCategoriesSelect_currentIndexChanged(int index);

private:
    Ui::CAccountingWidget *ui;
    void sumPriceTicketsForAction();
};

#endif // CACCOUNTINGWIDGET_H
