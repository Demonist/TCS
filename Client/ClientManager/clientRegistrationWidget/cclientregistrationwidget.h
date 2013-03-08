#ifndef CCLIENTREGISTRATIONWIDGET_H
#define CCLIENTREGISTRATIONWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "cdatedialog.h"
#include "cstatistics.h"
#include "cmarket.h"
#include "global.h"

namespace Ui {
class CClientRegistrationWidget;
}

class CClientRegistrationWidget : public CAbstractCommonWidget
{
    Q_OBJECT
    
public:
    explicit CClientRegistrationWidget(QWidget *parent = 0);
    ~CClientRegistrationWidget();

public slots:
    void updateData();

private slots:
    void on_pbnSave_clicked();

private:
    Ui::CClientRegistrationWidget *ui;
    bool validateLogin(QString login);
    QString generatePassword();
};

#endif // CCLIENTREGISTRATIONWIDGET_H
