#ifndef CDATEDIALOG_H
#define CDATEDIALOG_H

#include <QtGui>

namespace Ui {
    class CDateDialog;
}

class CDateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDateDialog(QWidget *parent = 0);
	explicit CDateDialog(const QDate &date, QWidget *parent = 0);
    ~CDateDialog();

	QDate selectedDate() const;

private slots:
	void on_pbnCancel_clicked();
	void on_pbnOk_clicked();

public slots:
	void setDate(const QDate &date);

private:
    Ui::CDateDialog *ui;
	QDate mSelectedDate;
};

#endif // CDATEDIALOG_H
