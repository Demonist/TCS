#ifndef CADMINSTARTINGDIALOG_H
#define CADMINSTARTINGDIALOG_H

#include <QtGui>
#include <QtSql>
#include "admin.h"

namespace Ui {
	class CAdminStartingDialog;
}

class CAdminStartingDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit CAdminStartingDialog(const QString &connectionName, QWidget *parent = 0);
	~CAdminStartingDialog();
	
	inline bool isLogined() const;

private slots:
	void connected(const QString &connectionName);
	void on_pbnExit_clicked();
	void on_pbnLogin_clicked();
	void on_lePassword_returnPressed();

private:
	Ui::CAdminStartingDialog *ui;
	QString mConnectionName;
	bool mLogined;
};

//inline implementaion:

inline bool CAdminStartingDialog::isLogined() const
{
	return mLogined;
}

#endif // CADMINSTARTINGDIALOG_H
