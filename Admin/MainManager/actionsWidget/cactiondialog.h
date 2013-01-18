#ifndef CACTIONDIALOG_H
#define CACTIONDIALOG_H

#include <QtGui>
#include <QtSql>
#include "global.h"
#include "cdatedialog.h"
#include "cactionticketsmanagement.h"

namespace Ui {
    class CActionDialog;
}

class CActionDialog : public QDialog
{
    Q_OBJECT
private:
	enum Type{Add, Edit};

private:
	Ui::CActionDialog *ui;
	Type mType;
	int mId;	//! Идентификатор мероприятия.
	QString mConnectionName;

	QStringListModel mCompleterModel;
	QCompleter mCompleter;

public:
	explicit CActionDialog(const QString &connectionName, QWidget *parent = 0);
	explicit CActionDialog(const QString &connectionName, const int id, QWidget *parent = 0);
    ~CActionDialog();

private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();
	void on_tbnDate_clicked();
	void on_pbnTicketsManagement_clicked();

signals:
	void dataWasUpdated();
};

#endif // CACTIONDIALOG_H
