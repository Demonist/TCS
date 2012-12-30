#ifndef CCATEGORIESWIDGET_H
#define CCATEGORIESWIDGET_H

#include <QtGui>
#include "cabstractcommonwidget.h"
#include "ccategorydialog.h"

namespace Ui {
class CCategoriesWidget;
}

class CCategoriesWidget : public CAbstractCommonWidget
{
	Q_OBJECT
private:
	Ui::CCategoriesWidget *ui;

public:
	explicit CCategoriesWidget(QWidget *parent = 0);
	~CCategoriesWidget();

public slots:
	void updateData();

private slots:
	void on_tbnCategoriesAdd_clicked();
	void on_tbnCategoriesEdit_clicked();
	void on_tbnCategoriesDelete_clicked();
};

#endif // CCATEGORIESWIDGET_H
