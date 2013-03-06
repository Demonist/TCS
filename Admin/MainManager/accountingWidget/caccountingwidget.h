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

private:
    Ui::CAccountingWidget *ui;
};

#endif // CACCOUNTINGWIDGET_H
