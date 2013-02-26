#include <QtGui/QApplication>
#include "controlmanagermainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    ControlManagerMainWindow w;
    w.show();
    
    return a.exec();
}
