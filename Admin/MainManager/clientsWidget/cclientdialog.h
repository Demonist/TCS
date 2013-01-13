#ifndef CCLIENTDIALOG_H
#define CCLIENTDIALOG_H

#include <QtGui>
#include <QtSql>

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
    bool validateLogin(QString login, Type actionType, int id = 0);

public:
	explicit CClientDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
	explicit CClientDialog(const QString &connectionName, const int id, QWidget *parent = 0);
	~CClientDialog();


private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

signals:
	void dataWasUpdated();
};

#endif // CCLIENTDIALOG_H
