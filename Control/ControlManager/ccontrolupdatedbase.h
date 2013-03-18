#ifndef CCONTROLUPDATEDBASE_H
#define CCONTROLUPDATEDBASE_H

#include <QDialog>

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
};

#endif // CCONTROLUPDATEDBASE_H
