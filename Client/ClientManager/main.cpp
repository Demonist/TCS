#include <QtGui/QApplication>
#include <QTextCodec>
#include "cclientmainwindow.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QApplication a(argc, argv);
	CClientMainWindow w;
	
	return a.exec();
}
