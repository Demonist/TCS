#ifndef CPRICEDIALOG_H
#define CPRICEDIALOG_H

#include <QtGui>
#include <QtSql>

namespace Ui {
	class CPriceDialog;
}

class CPriceDialog : public QDialog
{
	Q_OBJECT
	
	enum Type{Add, Edit};

private:
	void setColor(const QString &color);

public:
	explicit CPriceDialog(const QString &connectionName, const int actionId, QWidget *parent = 0);
	explicit CPriceDialog(const QString &connectionName, const int actionId, const int priceGroupId, QWidget *parent = 0);
	~CPriceDialog();
	
	inline int id() const;
	QString name() const;
	int price() const;
	int penalty() const;
	inline QColor color() const;

private slots:
	void on_pbnCancel_clicked();
	void on_pbnApply_clicked();
	void on_tbnColor_clicked();

private:
	Ui::CPriceDialog *ui;
	Type mType;
	QString mConnectionName;
	int mId;		//! Идентификатор ценовой группы.
	int mActionId;	//! Идентификатор меропрятия.
	QString mColor;
};

//inline implementation:

/**
Возвращает идентификатор ценовой группы.
Данную функцию полезно применять для получения идентификатора добавленной ценовой группы.
*/
inline int CPriceDialog::id() const
{
	return mId;
}

inline QColor CPriceDialog::color() const
{
	return QColor(mColor);
}
#endif // CPRICEDIALOG_H
