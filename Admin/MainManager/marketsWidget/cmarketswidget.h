#ifndef CMARKETSWIDGET_H
#define CMARKETSWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cmarketdialog.h"

namespace Ui {
class CMarketsWidget;
}

class CMarketsWidget : public CAbstractCommonWidget
{
    Q_OBJECT
    
public:
    explicit CMarketsWidget(QWidget *parent = 0);
    ~CMarketsWidget();
    
private:
    Ui::CMarketsWidget *ui;

public slots:
    void updateData();

private slots:
    void on_tbnMarketAdd_clicked();
    void on_tbnMarketEdit_clicked();
    void on_tbnMarketDelete_clicked();
};

#endif // CMARKETSWIDGET_H
