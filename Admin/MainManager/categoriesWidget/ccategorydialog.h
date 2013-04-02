#ifndef CCATEGORYDIALOG_H
#define CCATEGORYDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
class CCategoryDialog;
}

class CCategoryDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::CCategoryDialog *ui;
	enum Type{Add, Edit};
	Type mType;
	int mId;
	QString mConnectionName;

public:
	explicit CCategoryDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
	explicit CCategoryDialog(const QString &connectionName, const int id, QWidget *parent = 0);
	~CCategoryDialog();

private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();

signals:
	void dataWasUpdated();
};

#endif // CCATEGORYDIALOG_H
