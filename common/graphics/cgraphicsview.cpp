#include "cgraphicsview.h"

#define SCALLING_TABLE_SIZE 19
const static qreal gScallingTable[SCALLING_TABLE_SIZE] = {
	0.2f, 0.4f, 0.6f, 0.8f,
	1.0f,
	1.2f, 1.4f, 1.6f, 1.9f, 2.2f, 2.6f, 3.1f, 3.7f, 4.4f, 5.2f, 6.2f, 7.4f, 8.8f, 10.0f
};

qreal CGraphicsView::roundScale(const qreal &scale) const
{
	int left = 0;
	int right = SCALLING_TABLE_SIZE - 1;

	if(scale < gScallingTable[left])
		return gScallingTable[left];
	else if(scale > gScallingTable[right])
		return gScallingTable[right];
	else
	{
		int midle = right / 2;

		while(midle != left && midle != right)
		{
			if(scale < gScallingTable[midle])
				right = midle;
			else
				left = midle;
			midle = (right + left) / 2;
		}

		if(qAbs(gScallingTable[left] - scale) <= qAbs(gScallingTable[right] - scale))
		{
			if(gScallingTable[left] != mScale)
				return gScallingTable[left];
			else
				return gScallingTable[right];
		}
		else
		{
			if(gScallingTable[right] != mScale)
				return gScallingTable[right];
			else
				return gScallingTable[left];
		}
	}
}

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
				scaleUp();
			else
				scaleDown();
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
	mWheelScallingAnimated = true;
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

void CGraphicsView::scaleUp()
{
	if(mScale < gScallingTable[SCALLING_TABLE_SIZE - 1])
	{
		qreal newScale = mScale;
		if(mScaleAnimation.state() == QPropertyAnimation::Running
		   && mScaleAnimation.startValue().toReal() < mScaleAnimation.endValue().toReal())
			newScale = mScaleAnimation.endValue().toReal();
		newScale *= 1.2f;
		newScale = roundScale(newScale);
		if(mWheelScallingAnimated)
			setScaleAnimated(newScale);
		else
			setScale(newScale);
		viewport()->repaint();
	}
}

void CGraphicsView::scaleDown()
{
	if(mScale > gScallingTable[0])
	{
		qreal newScale = mScale;
		if(mScaleAnimation.state() == QPropertyAnimation::Running
		   && mScaleAnimation.startValue().toReal() > mScaleAnimation.endValue().toReal())
			newScale = mScaleAnimation.endValue().toReal();
		newScale *= 0.8f;
		newScale = roundScale(newScale);
		if(mWheelScallingAnimated)
			setScaleAnimated(newScale);
		else
			setScale(newScale);
		viewport()->repaint();
	}
}

//public slots:

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
