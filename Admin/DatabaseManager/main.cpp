#include <QtGui/QApplication>
#include <QTextCodec>
#include "cdatabasemanager.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QApplication a(argc, argv);
	CDatabaseManager w;
	w.show();
	
	return a.exec();
}
