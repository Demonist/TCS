#include "cabstractgraphicsitem.h"

CAbstractGraphicsItem::CAbstractGraphicsItem() :
	QGraphicsObject()
{
	mScaleAnimation.setTargetObject(this);
	mScaleAnimation.setPropertyName("scale");
	mScaleAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mOpacityAnimation.setTargetObject(this);
	mOpacityAnimation.setPropertyName("opacity");
	mOpacityAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mBrushColorAnimation.setTargetObject(this);
	mBrushColorAnimation.setPropertyName("brushColor");
	mBrushColorAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mPenColorAnimation.setTargetObject(this);
	mPenColorAnimation.setPropertyName("penColor");
	mPenColorAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mPenWidthAnimation.setTargetObject(this);
	mPenWidthAnimation.setPropertyName("penWidth");
	mPenWidthAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mBlurEffect.setBlurRadius(0.0);
	setGraphicsEffect(&mBlurEffect);
	mBlurAnimation.setTargetObject(this);
	mBlurAnimation.setPropertyName("blurRadius");
	mBlurAnimation.setEasingCurve(QEasingCurve::InOutSine);
}

CAbstractGraphicsItem::~CAbstractGraphicsItem()
{

}

void CAbstractGraphicsItem::setScaleAnimated(const qreal newScale, const int durationMs)
{
	mScaleAnimation.stop();
	mScaleAnimation.setStartValue(scale());
	mScaleAnimation.setEndValue(newScale);
	mScaleAnimation.setDuration(durationMs);
	mScaleAnimation.start();
}

void CAbstractGraphicsItem::setOpacityAnimated(const qreal newOpacity, const int durationMs)
{
	mOpacityAnimation.stop();
	mOpacityAnimation.setStartValue(opacity());
	mOpacityAnimation.setEndValue(newOpacity);
	mOpacityAnimation.setDuration(durationMs);
	mOpacityAnimation.start();
}

void CAbstractGraphicsItem::setBrushColorAnimated(const QColor &newBrushColor, const int durationMs)
{
	mBrushColorAnimation.stop();
	mBrushColorAnimation.setStartValue(brushColor());
	mBrushColorAnimation.setEndValue(newBrushColor);
	mBrushColorAnimation.setDuration(durationMs);
	mBrushColorAnimation.start();
}

void CAbstractGraphicsItem::setPenColorAnimated(const QColor &newPenColor, const int durationMs)
{
	mPenColorAnimation.stop();
	mPenColorAnimation.setStartValue(penColor());
	mPenColorAnimation.setEndValue(newPenColor);
	mPenColorAnimation.setDuration(durationMs);
	mPenColorAnimation.start();
}

void CAbstractGraphicsItem::setPenWidthAnimated(const int newPenWidth, const int durationMs)
{
	mPenWidthAnimation.stop();
	mPenWidthAnimation.setStartValue(penWidth());
	mPenWidthAnimation.setEndValue(newPenWidth);
	mPenWidthAnimation.setDuration(durationMs);
	mPenWidthAnimation.start();
}

void CAbstractGraphicsItem::setBlurAnimated(const qreal newBlurRadius, const int durationMs)
{
	mBlurAnimation.stop();
	mBlurAnimation.setStartValue(blurRadius());
	mBlurAnimation.setEndValue(newBlurRadius);
	mBlurAnimation.setDuration(durationMs);
	mBlurAnimation.start();
}
