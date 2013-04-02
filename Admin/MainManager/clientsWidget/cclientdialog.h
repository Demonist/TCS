#ifndef CCLIENTDIALOG_H
#define CCLIENTDIALOG_H

#include <QtGui>
#include <QtSql>
#include "global.h"

namespace Ui {
class CClientDialog;
}

class CClientDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::CClientDialog *ui;
	enum Type{Add, Edit};
	Type mType;
	int mId;
	QString mConnectionName;

private:
	bool validateLogin(QString login, Type actionType);

public:
	explicit CClientDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
	explicit CClientDialog(const QString &connectionName, const int id, QWidget *parent = 0);	//Конструктор редактирования.
	~CClientDialog();


private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();

signals:
	void dataWasUpdated();
};

#endif // CCLIENTDIALOG_H
