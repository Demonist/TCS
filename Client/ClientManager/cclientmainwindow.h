#ifndef CCLIENTMAINWINDOW_H
#define CCLIENTMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "cstartingdialog.h"
#include "cimages.h"
#include "cclientactiondialog.h"

namespace Ui {
	class CClientMainWindow;
}

class CClientMainWindow : public QMainWindow
{
	Q_OBJECT
	
private:
	void closeEvent(QCloseEvent *event);

public:
	explicit CClientMainWindow(QWidget *parent = 0);
	~CClientMainWindow();
	
private slots:
	void on_listWidget_currentRowChanged(int currentRow);

private:
	Ui::CClientMainWindow *ui;
	QString mConnectionName;
	CClientActionDialog mActionDialog;
	bool mCanClose;
};

#endif // CCLIENTMAINWINDOW_H
