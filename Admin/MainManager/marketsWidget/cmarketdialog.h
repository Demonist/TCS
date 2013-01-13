#ifndef CMARKETDIALOG_H
#define CMARKETDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
class CMarketDialog;
}

class CMarketDialog : public QDialog
{
    Q_OBJECT
    
private:
    Ui::CMarketDialog *ui;
    enum Type{Add, Edit};
    Type mType;
    int mId;
    QString mConnectionName;

public:
    explicit CMarketDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
    explicit CMarketDialog(const QString &connectionName, const int id, QWidget *parent = 0);
    ~CMarketDialog();

signals:
    void dataWasUpdated();
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // CMARKETDIALOG_H
