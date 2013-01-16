#ifndef CBACKGROUNDDIALOG_H
#define CBACKGROUNDDIALOG_H

#include <QtGui>
#include <QtSql>
#include "cimages.h"

namespace Ui {
	class CBackgroundDialog;
}

class CBackgroundDialog : public QDialog
{
	Q_OBJECT
	
private:
	void updateImage();
	void resizeEvent(QResizeEvent *event);

public:
	explicit CBackgroundDialog(const QString &connectionName, const int placeId, QWidget *parent = 0);
	~CBackgroundDialog();

	inline bool wasUpdated() const;
	inline QPixmap image() const;
	
private slots:
	void on_pbnCancel_clicked();
	void on_pbnSelectNew_clicked();
	void on_pbnRemove_clicked();
	void on_pbnApply_clicked();

private:
	Ui::CBackgroundDialog *ui;
	QString mConnectionName;
	int mId;	//! Идентификатор площадки.
	QPixmap mImage;
	QString mImageFileName;
	bool mWasChanged;
};

//inline implementations:

inline bool CBackgroundDialog::wasUpdated() const
{
	return mWasChanged;
}

inline QPixmap CBackgroundDialog::image() const
{
	return mImage;
}

#endif // CBACKGROUNDDIALOG_H
