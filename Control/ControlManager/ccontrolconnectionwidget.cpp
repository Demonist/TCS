#include "ccontrolconnectionwidget.h"

QString CControlConnectionWidget::selectDatabaseFile()
{
	static QString importPath;
	if(importPath.isEmpty())
		importPath = Global::currentPath() + "/import";
	QDir().mkpath(importPath);

	return QFileDialog::getOpenFileName(this, tr("Выберите файл базы данных"), importPath, tr("Файл базы данных (*.db)"));
}

CControlConnectionWidget::CControlConnectionWidget(QWidget *parent) :
	CDataBaseConnectionWidget(parent)
{
}
