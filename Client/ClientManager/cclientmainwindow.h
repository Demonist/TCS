#ifndef CCLIENTMAINWINDOW_H
#define CCLIENTMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "cstartingdialog.h"

namespace Ui {
	class CClientMainWindow;
}

class CClientMainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit CClientMainWindow(QWidget *parent = 0);
	~CClientMainWindow();
	
private:
	Ui::CClientMainWindow *ui;
	QString mConnectionName;
};

#endif // CCLIENTMAINWINDOW_H
