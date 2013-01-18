#ifndef CUPLOADINGWIDGET_H
#define CUPLOADINGWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cdatedialog.h"
#include "cabstractcommonwidget.h"

namespace Ui {
class CUploadingWidget;
}

class CUploadingWidget : public CAbstractCommonWidget
{
    Q_OBJECT
    
public:
    explicit CUploadingWidget(QWidget *parent = 0);
    ~CUploadingWidget();
    
private:
    Ui::CUploadingWidget *ui;
    QString tDate;
    QString tActionName;


public slots:
    void updateData();
private slots:
    void on_twActions_itemSelectionChanged();
    void on_tbnSelectDate_clicked();
    void on_tbnSearch_clicked();
    void on_tbClearDate_clicked();
};

#endif // CUPLOADINGWIDGET_H
