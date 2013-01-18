#ifndef CACCOUNTINGWIDGET_H
#define CACCOUNTINGWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "ccategorydialog.h"
#include "caccountingactionsdialog.h"
#include "cdatedialog.h"

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
    void on_tbnActions_clicked();

    void on_tbnS_clicked();

    void on_tbnDo_clicked();

private:
    Ui::CAccountingWidget *ui;
};

#endif // CACCOUNTINGWIDGET_H
