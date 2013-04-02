#ifndef CPLACEDIALOG_H
#define CPLACEDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cplaceschemedialog.h"

namespace Ui {
class CPlaceDialog;
}

class CPlaceDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::CPlaceDialog *ui;
	enum Type{Add, Edit};
	Type mType;
	int mId;
	QString mConnectionName;

public:
	explicit CPlaceDialog(const QString &connectionName, QWidget *parent = 0);   //Конструктор добавления.
	explicit CPlaceDialog(const QString &connectionName, const int id, QWidget *parent = 0);
	~CPlaceDialog();

signals:
	void dataWasUpdated();

private slots:
	void on_pbnEditPlaceScheme_clicked();
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();
};

#endif // CPLACEDIALOG_H
