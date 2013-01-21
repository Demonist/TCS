#ifndef CONTROLMANAGERMAINWINDOW_H
#define CONTROLMANAGERMAINWINDOW_H

#include <QMainWindow>

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
    void on_lineEdit_returnPressed();

private:
    Ui::ControlManagerMainWindow *ui;
};

#endif // CONTROLMANAGERMAINWINDOW_H
