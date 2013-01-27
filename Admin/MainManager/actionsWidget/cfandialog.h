#ifndef CFANDIALOG_H
#define CFANDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
	class CFanDialog;
}

class CFanDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit CFanDialog(const QString connectionName, const int actionId, QWidget *parent = 0);
	~CFanDialog();

	int price() const;
	int penalty() const;
	int count() const;
	
private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();

private:
	Ui::CFanDialog *ui;
	QString mConnectionName;
	int mId;	//! Идентификатор мероприятия.
};

#endif // CFANDIALOG_H
