#include "csourcegraphicsview.h"

//protected:

void CSourceGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	CGraphicsView::mouseMoveEvent(event);

	if(mDragging && horizontalScrollBar() && verticalScrollBar())
		emit dragChanged(horizontalScrollBar()->value(), verticalScrollBar()->value());
}

//public:

CSourceGraphicsView::CSourceGraphicsView(QWidget *parent) :
	CGraphicsView(parent)
{
}

void CSourceGraphicsView::setScale(const qreal scale)	//Do not make inline. Never! Otherwise recursion arise.
{
	CGraphicsView::setScale(scale);
	emit scaleChanged(scale);
}
