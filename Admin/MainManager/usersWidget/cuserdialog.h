#ifndef CUSERDIALOG_H
#define CUSERDIALOG_H

#include <QtGui>
#include <QtSql>
#include "global.h"

namespace Ui {
class CUserDialog;
}

class CUserDialog : public QDialog
{
	Q_OBJECT
private:
	enum Type{Add, Edit};

private:
	Ui::CUserDialog *ui;
	Type mType;
	int mId;
	QString mConnectionName;

public:
	explicit CUserDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
	explicit CUserDialog(const QString &connectionName, const int id, QWidget *parent = 0); //Конструктор редактирвоания.
	~CUserDialog();

private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

signals:
	void dataWasUpdated();
};

#endif // CUSERDIALOG_H
