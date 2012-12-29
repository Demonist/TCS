#ifndef CCLIENTSWIDGET_H
#define CCLIENTSWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cclientdialog.h"

namespace Ui {
class CClientsWidget;
}

class CClientsWidget : public CAbstractCommonWidget
{
    Q_OBJECT
    
public:
    explicit CClientsWidget(QWidget *parent = 0);
    ~CClientsWidget();
public slots:
    void updateData();
private slots:
    void on_pbClientsAdd_clicked();

    void on_pbClientsEdit_clicked();

    void on_pbClientsDelete_clicked();

private:
    Ui::CClientsWidget *ui;
};

#endif // CCLIENTSWIDGET_H
