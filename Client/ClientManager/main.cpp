#include <QtGui/QApplication>
#include <QTextCodec>
#include "cclientmainwindow.h"
#include "global.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QApplication a(argc, argv);

	QDir path(Global::currentPath());
	path.cd("plugins");
	QApplication::addLibraryPath(path.absolutePath());

	CClientMainWindow w;
	
	return a.exec();
}
