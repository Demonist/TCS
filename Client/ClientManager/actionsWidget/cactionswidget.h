#ifndef CACTIONSWIDGET_H
#define CACTIONSWIDGET_H

#include <QWidget>

namespace Ui {
	class CActionsWidget;
}

class CActionsWidget : public QWidget
{
	Q_OBJECT
	
public:
	explicit CActionsWidget(QWidget *parent = 0);
	~CActionsWidget();
	
private:
	Ui::CActionsWidget *ui;
};

#endif // CACTIONSWIDGET_H
