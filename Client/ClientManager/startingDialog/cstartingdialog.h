#ifndef CSTARTINGDIALOG_H
#define CSTARTINGDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cdatabaseconnectionwidget.h"
#include "cmarket.h"

namespace Ui {
	class CStartingDialog;
}

class CStartingDialog : public QDialog
{
	Q_OBJECT
	
private:
	Ui::CStartingDialog *ui;
	bool mLogined;
	int mLoginedId;
	QString mConnectionName;

public:
	explicit CStartingDialog(const QString &connectionName, QWidget *parent = 0);
	~CStartingDialog();

	void setChangeUserOnly();

	inline QString connectionName() const;
	inline bool isLogined() const;
	inline int loginedId() const;
	
private slots:
	void connected(const QString& connectionName);
	void on_pbnExit_clicked();
	void on_pbnLogin_clicked();
	void on_leLogin_returnPressed();
	void on_lePassword_returnPressed();
};

//inline implementation:

inline QString CStartingDialog::connectionName() const
{
	return mConnectionName;
}

inline bool CStartingDialog::isLogined() const
{
	return mLogined;
}

inline int CStartingDialog::loginedId() const
{
	return mLoginedId;
}

#endif // CSTARTINGDIALOG_H
