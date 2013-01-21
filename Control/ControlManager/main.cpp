#include <QtGui/QApplication>
#include "controlmanagermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlManagerMainWindow w;
    w.show();
    
    return a.exec();
}
