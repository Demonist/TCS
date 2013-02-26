#ifndef CONTROLMANAGERMAINWINDOW_H
#define CONTROLMANAGERMAINWINDOW_H

#include <QtGui>
#include <QtSql>

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
    void openDataBase();
private:
    Ui::ControlManagerMainWindow *ui;
};

#endif // CONTROLMANAGERMAINWINDOW_H
