#ifndef CCLIENTREGISTRATIONWIDGET_H
#define CCLIENTREGISTRATIONWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "global.h"
#include "cdatedialog.h"

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

    void on_tbnBirthDate_clicked();

private:
    Ui::CClientRegistrationWidget *ui;
    bool validateLogin(QString login);
    QString generatePassword(QString hs);
};

#endif // CCLIENTREGISTRATIONWIDGET_H
