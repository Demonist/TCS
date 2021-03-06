#ifndef CIMAGEDIALOG_H
#define CIMAGEDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cimages.h"

namespace Ui {
	class CImageDialog;
}

/**
Класс диалога для универсальной работы с изображениями.
Позволяет отображать, выбирать новое и удалить изображения.
Все операции с изображениями производятся в базе данных.

\return Возвращает QDialog::Accepted если данные были изменены.

\author Demonist
*/
class CImageDialog : public QDialog
{
	Q_OBJECT
	
private:
	void updateImage();
	void resizeEvent(QResizeEvent *event);

public:
	explicit CImageDialog(const QString &connectionName, QWidget *parent = 0);
	explicit CImageDialog(const QString &connectionName, const int imageId, QWidget *parent = 0);
	explicit CImageDialog(const QString &connectionName, const QString &tableName, const QString &imageIdFieldName, const QString &condition, QWidget *parent = 0);
	~CImageDialog();

	inline QPixmap image() const;
	inline int id() const;
	
private slots:
	void on_pbnCancel_clicked();
	void on_pbnSelectNew_clicked();
	void on_pbnRemove_clicked();
	void on_pbnApply_clicked();

private:
	Ui::CImageDialog *ui;
	QString mConnectionName;
	int mId;	//! Идентификатор изображения.
	QPixmap mImage;
	QString mImageFileName;
	bool mWasChanged;
	bool mAlternativeConstructor;
	QString mTable;
	QString mField;
	QString mCondition;
};

//inline implementations:

inline QPixmap CImageDialog::image() const
{
	return mImage;
}

inline int CImageDialog::id() const
{
	return mId;
}

#endif // CIMAGEDIALOG_H
