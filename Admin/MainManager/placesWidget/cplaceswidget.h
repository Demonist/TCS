#ifndef CPLACESWIDGET_H
#define CPLACESWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cplacedialog.h"

namespace Ui {
class CPlacesWidget;
}

class CPlacesWidget : public CAbstractCommonWidget
{
	Q_OBJECT
private:
	Ui::CPlacesWidget *ui;

public:
	explicit CPlacesWidget(QWidget *parent = 0);
	~CPlacesWidget();

public slots:
	void updateData();

private slots:
	void on_pbPlacesAdd_clicked();
	void on_pbPlacesEdit_clicked();
	void on_pbPlacesDelete_clicked();


};

#endif // CPLACESWIDGET_H
