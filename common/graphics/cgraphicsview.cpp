#include "cgraphicsview.h"

//protected:

void CGraphicsView::paintEvent(QPaintEvent *event)
{
	QGraphicsView::paintEvent(event);

	if(mLegend)
	{
		QPainter painter(viewport());
		painter.setRenderHint(QPainter::Antialiasing);
		QSize s = size();
		if(horizontalScrollBar() && horizontalScrollBar()->isVisible())
			s.setHeight(s.height() - horizontalScrollBar()->height());
		if(verticalScrollBar() && verticalScrollBar()->isVisible())
			s.setWidth(s.width() - verticalScrollBar()->width());
		mLegend->paintLenend(&painter, s);
	}
}

void CGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton
	   || (mDrag && event->button() == Qt::LeftButton))
	{
		mDragging = true;
		mDragPos = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
	QGraphicsView::mousePressEvent(event);
}

void CGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	if(mDragging)
	{
		if(event->button() == Qt::RightButton
		   || (mDrag && event->button() == Qt::LeftButton))
		{
			mDragging = false;
			if(mDrag)
				setCursor(Qt::OpenHandCursor);
			else
				setCursor(Qt::ArrowCursor);
		}
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void CGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if(mDragging)
	{
		QPoint scroll = mDragPos - event->pos();
		if(horizontalScrollBar())
			horizontalScrollBar()->setValue(horizontalScrollBar()->value() + scroll.x());
		if(verticalScrollBar())
			verticalScrollBar()->setValue(verticalScrollBar()->value() + scroll.y());
		mDragPos = event->pos();
		viewport()->repaint();
	}
	QGraphicsView::mouseMoveEvent(event);
}

void CGraphicsView::wheelEvent(QWheelEvent *event)
{
	if(mWheelScalling)
	{
		if(event->orientation() == Qt::Vertical)
		{
			if(event->delta() > 0)
				emit wheelUp();
			else
				emit wheelDown();
		}
	}
	else
		QGraphicsView::wheelEvent(event);
}

//public:

CGraphicsView::CGraphicsView(QWidget *parent) :
	QGraphicsView(parent)
{
	viewport()->setMouseTracking(true);

	mDragging = false;
	mDrag = false;
	mWheelScalling = true;
	mScale = 1.0f;
	mLegend = 0;

	mScaleAnimation.setTargetObject(this);
	mScaleAnimation.setPropertyName("scale");
	mScaleAnimation.setEasingCurve(QEasingCurve::InOutSine);
}

CGraphicsView::~CGraphicsView()
{
	mScaleAnimation.stop();
}

void CGraphicsView::setDragEnabled(const bool enableDrag)
{
	mDrag = enableDrag;

	if(mDrag)
		setCursor(Qt::OpenHandCursor);
	else
	{
		mDragging = false;
		setCursor(Qt::ArrowCursor);
	}
}

void CGraphicsView::setScale(const qreal scale)
{
	if(mScale != scale)
	{
		mScale = scale;
		setTransform(QTransform::fromScale(mScale, mScale));
	}
}

void CGraphicsView::setScaleAnimated(const qreal scale, const int durationMs)
{
	if(mScale != scale)
	{
		mScaleAnimation.stop();
		mScaleAnimation.setStartValue(mScale);
		mScaleAnimation.setEndValue(scale);
		mScaleAnimation.setDuration(durationMs);
		mScaleAnimation.start();
	}
}
