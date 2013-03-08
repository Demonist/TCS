#ifndef CCLIENTMAINWINDOW_H
#define CCLIENTMAINWINDOW_H

#include <QtGui>
#include <QtSql>
#include "cstartingdialog.h"
#include "cimages.h"
#include "cstatistics.h"
#include "cclientactiondialog.h"
#include "cmarket.h"

namespace Ui {
	class CClientMainWindow;
}

class CClientMainWindow : public QMainWindow
{
	Q_OBJECT

private:
	void closeEvent(QCloseEvent *event);
	void createActionDialog();

public:
	explicit CClientMainWindow(QWidget *parent = 0);
	~CClientMainWindow();

private slots:
	void hideLeftPanel();
	void showLeftPanel();
	void on_listWidget_currentRowChanged(int currentRow);
	void on_pbnChangeUser_clicked();

private:
	Ui::CClientMainWindow *ui;
	QString mConnectionName;
	CClientActionDialog mActionDialog;
	bool mCanClose;
};

#endif // CCLIENTMAINWINDOW_H
