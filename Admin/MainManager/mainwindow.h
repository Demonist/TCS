#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cimages.h"
#include "cadminstartingdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow *ui;
	QString mConnectionName;

private:
	void createTables();

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_lwSettings_currentRowChanged(int currentRow);
};

#endif // MAINWINDOW_H
