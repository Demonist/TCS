#ifndef CCLIENTACTIONDIALOG_H
#define CCLIENTACTIONDIALOG_H

#include <QtGui>
#include <QtSql>
#include "csourcegraphicsview.h"
#include "cimages.h"
#include "cadvertisementitem.h"

namespace Ui {
	class CClientActionDialog;
}

class CClientActionDialog : public QDialog
{
	Q_OBJECT
	
protected:
	void closeEvent(QCloseEvent *event);

public:
	explicit CClientActionDialog(QWidget *parent = 0);
	~CClientActionDialog();

	void setSourceView(CSourceGraphicsView *sourceView);

private slots:
	void changeItem();

public slots:
	void setScale(const qreal scale);
	void drag(int x, int y);
	void updateAdvertisements(const QString &connectionName);
	inline void setCanClose(const bool canClose);
	void show();
	void showSeats();
	void showAdvertisements();

private:
	Ui::CClientActionDialog *ui;
	bool mCanClose;
	bool mOnSecondScreen;

	QTimer mAdvertisementTimer;
	QGraphicsScene mAdvertisementScene;
	QMutex mAdvertisementMutex;
	QList<CAdvertisementItem*> mAdvertisementItems;
	int mAdvertisementItemCurrentIndex;
};

//inline implementation:

inline void CClientActionDialog::setCanClose(const bool canClose)
{
	mCanClose = canClose;
}

#endif // CCLIENTACTIONDIALOG_H
