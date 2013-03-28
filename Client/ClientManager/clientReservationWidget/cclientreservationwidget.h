#ifndef CCLIENTRESERVATIONWIDGET_H
#define CCLIENTRESERVATIONWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "global.h"

namespace Ui {
	class CClientReservationWidget;
}

class CClientReservationWidget : public CAbstractCommonWidget
{
	Q_OBJECT
	
public:
	explicit CClientReservationWidget(QWidget *parent = 0);
	~CClientReservationWidget();
	
public slots:
	void updateData();

private slots:
	void on_tbnClearClient_clicked();
	void on_leClient_textChanged(const QString &arg1);
	void on_pbnReservationManagement_clicked();
	void on_pbnSelectAll_clicked();
	void on_pbnUnreservate_clicked();
	void on_pbnSoldReservated_clicked();
	void on_pbnBackToClients_clicked();

private:
	Ui::CClientReservationWidget *ui;
};

#endif // CCLIENTRESERVATIONWIDGET_H
