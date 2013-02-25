#ifndef CCLIENTACTIONDIALOG_H
#define CCLIENTACTIONDIALOG_H

#include <QtGui>
#include "csourcegraphicsview.h"

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

public slots:
	void setScale(const qreal scale);
	void drag(int x, int y);
	inline void setCanClose(const bool canClose);

private:
	Ui::CClientActionDialog *ui;
	bool mCanClose;
};

//inline implementation:

inline void CClientActionDialog::setCanClose(const bool canClose)
{
	mCanClose = canClose;
}

#endif // CCLIENTACTIONDIALOG_H
