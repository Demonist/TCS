#include "controlmanagermainwindow.h"
#include "ui_controlmanagermainwindow.h"

ControlManagerMainWindow::ControlManagerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControlManagerMainWindow)
{
    ui->setupUi(this);
}

ControlManagerMainWindow::~ControlManagerMainWindow()
{
    delete ui;
}

void ControlManagerMainWindow::on_lineEdit_returnPressed()
{
    if(ui->lineEdit->text() == "123")
    {
        ui->widget->setStyleSheet("background: green");
    }
}
