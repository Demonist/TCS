#ifndef CADVERTISEMENTSWIDGET_H
#define CADVERTISEMENTSWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "cimages.h"
#include "cimagedialog.h"
#include "ccachechecker.h"

namespace Ui {
	class CAdvertisementsWidget;
}

class CAdvertisementsWidget : public CAbstractCommonWidget
{
	Q_OBJECT
	
public:
	explicit CAdvertisementsWidget(QWidget *parent = 0);
	~CAdvertisementsWidget();

	void updateData();
	
private slots:
	void on_tbnAdd_clicked();
	void on_tbnDel_clicked();

private:
	Ui::CAdvertisementsWidget *ui;
};

#endif // CADVERTISEMENTSWIDGET_H
