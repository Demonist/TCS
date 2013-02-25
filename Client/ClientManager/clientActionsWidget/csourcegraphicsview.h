#ifndef CSOURCEGRAPHICSVIEW_H
#define CSOURCEGRAPHICSVIEW_H

#include <QtGui>
#include "cgraphicsview.h"

class CSourceGraphicsView : public CGraphicsView
{
	Q_OBJECT
protected:
	void mouseMoveEvent(QMouseEvent *event);

public:
	explicit CSourceGraphicsView(QWidget *parent = 0);
	
signals:
	void scaleChanged(qreal newScale);
	void dragChanged(int horizontalBarValue, int vertiacalBarValue);

public slots:
	void setScale(const qreal scale);
};

#endif // CSOURCEGRAPHICSVIEW_H
