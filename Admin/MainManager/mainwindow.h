#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cimages.h"
#include "cadminstartingdialog.h"
#include "caboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindow *ui;
	QString mConnectionName;

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_lwSettings_currentRowChanged(int currentRow);
	void on_pbnAbout_clicked();
};

#endif // MAINWINDOW_H
