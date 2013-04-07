#ifndef CCONTROLUPDATEDBASE_H
#define CCONTROLUPDATEDBASE_H

#include <QtGui>
#include "uploading.h"

namespace Ui {
class CControlUpdateDBase;
}

class CControlUpdateDBase : public QDialog
{
	Q_OBJECT
	
public:
	explicit CControlUpdateDBase(QWidget *parent = 0);
	~CControlUpdateDBase();
	
private:
	Ui::CControlUpdateDBase *ui;
	QString mConnectionName;

private slots:
	void connected(QString connectionName);
	void on_pbnImport_clicked();
};

#endif // CCONTROLUPDATEDBASE_H
