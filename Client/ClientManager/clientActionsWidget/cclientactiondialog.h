#ifndef CCLIENTACTIONDIALOG_H
#define CCLIENTACTIONDIALOG_H

#include <QtGui>
#include "cgraphicsview.h"

namespace Ui {
	class CClientActionDialog;
}

class CClientActionDialog : public QDialog
{
	Q_OBJECT
	
private:
	bool eventFilter(QObject *object, QEvent *event);

public:
	explicit CClientActionDialog(QWidget *parent = 0);
	~CClientActionDialog();
	
private slots:
	void on_pushButton_clicked();
	void on_pushButton_2_clicked();

public slots:
	void show(CGraphicsView *view);

private:
	Ui::CClientActionDialog *ui;
	QGraphicsView *mView;
};

#endif // CCLIENTACTIONDIALOG_H
