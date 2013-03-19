#ifndef CRETURNTICKETWIDGET_H
#define CRETURNTICKETWIDGET_H

#include <QtGui>
#include <QtSql>
#include "cabstractcommonwidget.h"
#include "cticketidentifier.h"
#include "global.h"
#include "cstatistics.h"
#include "cmarket.h"

namespace Ui {
	class CReturnTicketWidget;
}

class CReturnTicketWidget : public CAbstractCommonWidget
{
	Q_OBJECT
	
private:
	void clear();

public:
	explicit CReturnTicketWidget(QWidget *parent = 0);
	~CReturnTicketWidget();
	
private slots:
	void updateData();
	void on_leIdentifier_textChanged(const QString &arg1);
	void on_leIdentifier_returnPressed();
	void on_tbnClearIdentifier_clicked();

	void on_pbnReturnTicket_clicked();

private:
	Ui::CReturnTicketWidget *ui;
	int mTicketId;
	int mPenalty;
	int mClientId;
	int mActionId;
	int mPlaceSchemeId;
};

#endif // CRETURNTICKETWIDGET_H
