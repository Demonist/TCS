#ifndef CACCOUNTINGACTIONSDIALOG_H
#define CACCOUNTINGACTIONSDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
class CAccountingActionsDialog;
}

class CAccountingActionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CAccountingActionsDialog(const QString &connectionName,QWidget *parent = 0);
    ~CAccountingActionsDialog();
    
private:
    Ui::CAccountingActionsDialog *ui;
    QString mConnectionName;

signals:
    void dataWasUpdated();
};

#endif // CACCOUNTINGACTIONSDIALOG_H
