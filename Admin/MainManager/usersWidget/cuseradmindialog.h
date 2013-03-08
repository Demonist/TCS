#ifndef CUSERADMINDIALOG_H
#define CUSERADMINDIALOG_H

#include <QtGui>
#include <QtSql>
#include "global.h"

namespace Ui {
	class CUserAdminDialog;
}

class CUserAdminDialog : public QDialog
{
	Q_OBJECT
	
private:
	enum Type{Add, Edit};

private:
	Ui::CUserAdminDialog *ui;
	Type mType;
	int mId;	//! Идентификатор администратора.
	QString mConnectionName;

public:
	explicit CUserAdminDialog(const QString &connectionName, QWidget *parent = 0);
	explicit CUserAdminDialog(const QString &connectionName, const int id, QWidget *parent = 0);
	~CUserAdminDialog();
	
private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();

signals:
	void dataWasUpdated();
};

#endif // CUSERADMINDIALOG_H
