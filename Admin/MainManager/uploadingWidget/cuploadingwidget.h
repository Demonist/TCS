#ifndef CUPLOADINGWIDGET_H
#define CUPLOADINGWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cdatedialog.h"
#include "cabstractcommonwidget.h"
#include "uploading.h"

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
    void createDBScheme();
    bool openConnection();
    bool validateDataBase();

public slots:
    void updateData();

private slots:
    void on_twActions_itemSelectionChanged();
    void on_tbnSelectDate_clicked();
    void on_tbClearDate_clicked();
    void on_pbnUploading_clicked();
	void on_tbnSearchClear_clicked();
	void on_leSearch_textChanged(const QString &text);
};

#endif // CUPLOADINGWIDGET_H
