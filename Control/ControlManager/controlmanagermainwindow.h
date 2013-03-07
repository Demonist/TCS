#ifndef CONTROLMANAGERMAINWINDOW_H
#define CONTROLMANAGERMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "cticketidentifier.h"

namespace Ui {
class ControlManagerMainWindow;
}

class ControlManagerMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ControlManagerMainWindow(QWidget *parent = 0);
    ~ControlManagerMainWindow();
    
private slots:
    void on_leGetBarcode_returnPressed();
    void connected(QString connectionName);

private:
    Ui::ControlManagerMainWindow *ui;
    QString mConnectionName;
};

#endif // CONTROLMANAGERMAINWINDOW_H
