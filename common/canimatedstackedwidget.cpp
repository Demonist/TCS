#include "canimatedstackedwidget.h"

//public:

CAnimatedStackedWidget::CAnimatedStackedWidget(QWidget *parent) :
	QStackedWidget(parent)
{
	mNextWidget = 0;

	connect(&mAnimationOut, SIGNAL(finished()), this, SLOT(firstAnimationFinished()));
	connect(&mAnimationIn, SIGNAL(finished()), this, SLOT(secondAnimationFinished()));

	mAnimationOut.setPropertyName("pos");
	mAnimationOut.setEasingCurve(QEasingCurve::InQuart);
	mAnimationIn.setPropertyName("pos");
	mAnimationIn.setEasingCurve(QEasingCurve::OutQuart);
}

CAnimatedStackedWidget::~CAnimatedStackedWidget()
{

}

//public slots:

void CAnimatedStackedWidget::setCurrentWidgetAnimated(QWidget *widget, const Direction direction, const int duration)
{
	if(widget
	   && (widget != currentWidget()
		   || mNextWidget))
	{
		mAnimationOut.stop();
		mAnimationIn.stop();

		if(mNextWidget)
			setCurrentWidget(mNextWidget);

		mDirection = direction;
		mNextWidget = widget;

		mAnimationOut.setDuration(duration / 2);
		mAnimationIn.setDuration(duration / 2);

		mAnimationOut.setTargetObject(currentWidget());
		mAnimationOut.setStartValue(QPoint(0, 0));

		QPoint pos;
		switch(mDirection)
		{
			case DirectionUp: pos = QPoint(0, -currentWidget()->height() - 50); break;
			case DirectionDown: pos = QPoint(0, currentWidget()->height() + 50); break;
			case DirectionLeft: pos = QPoint(-currentWidget()->width() - 50, 0); break;
			case DirectionRight: pos = QPoint(currentWidget()->width() + 50, 0); break;
		}

		mAnimationOut.setEndValue(pos);

		mAnimationOut.start();
	}
}

void CAnimatedStackedWidget::setCurrentWidgetAnimatedVertical(QWidget *widget, const VerticalDirection direction, const int duration)
{
	Direction exactlyDirection;
	switch(direction)
	{
		case VerticalAutoReverse: exactlyDirection = indexOf(widget) > currentIndex() ? DirectionDown : DirectionUp; break;
		case VerticalUp: exactlyDirection = DirectionUp; break;
		case VerticalDown: exactlyDirection = DirectionDown; break;
		case VerticalAuto:
		default:
			exactlyDirection = indexOf(widget) > currentIndex() ? DirectionUp : DirectionDown; break;
	}
	setCurrentWidgetAnimated(widget, exactlyDirection, duration);
}

void CAnimatedStackedWidget::setCurrentWidgetAnimatedHorizontal(QWidget *widget, const HorizontalDirection direction, const int duration)
{
	Direction exactlyDirection;
	switch(direction)
	{
		case HorizontalAutoReverse: exactlyDirection = indexOf(widget) > currentIndex() ? DirectionRight : DirectionLeft; break;
		case HorizontalLeft: exactlyDirection = DirectionLeft; break;
		case HorizontalRight: exactlyDirection = DirectionRight; break;
		case HorizontalAuto:
		default:
			exactlyDirection = indexOf(widget) > currentIndex() ? DirectionLeft : DirectionRight; break;
	}
	setCurrentWidgetAnimated(widget, exactlyDirection, duration);
}

void CAnimatedStackedWidget::slideVerticalNext(const int duration)
{
	if(currentIndex() + 1 < count())
		setCurrentIndexAnimated(currentIndex() + 1, DirectionUp, duration);
	else
		qWarning("CAnimatedStackedWidget::slideVerticalNext. No next index.");
}

void CAnimatedStackedWidget::slideVerticalPrev(const int duration)
{
	if(currentIndex() > 0)
		setCurrentIndexAnimated(currentIndex() - 1, DirectionDown, duration);
	else
		qWarning("CAnimatedStackedWidget::slideVerticalPrev. No prev index.");
}

void CAnimatedStackedWidget::slideHorizontalNext(const int duration)
{
	if(currentIndex() + 1 < count())
		setCurrentIndexAnimated(currentIndex() + 1, DirectionLeft, duration);
	else
		qWarning("CAnimatedStackedWidget::slideHorizontalNext. No next index.");
}

void CAnimatedStackedWidget::slideHorizontalPrev(const int duration)
{
	if(currentIndex() > 0)
		setCurrentIndexAnimated(currentIndex() - 1, DirectionRight, duration);
	else
		qWarning("CAnimatedStackedWidget::slideHorizontalPrev. No prev index.");
}

//private slots:

void CAnimatedStackedWidget::firstAnimationFinished()
{
	if(mNextWidget)
	{
		mAnimationIn.stop();

		mAnimationIn.setTargetObject(mNextWidget);

		QPoint pos;
		switch(mDirection)
		{
			case DirectionUp: pos = QPoint(0, mNextWidget->height() + 50); break;
			case DirectionDown: pos = QPoint(0, -mNextWidget->height() - 50); break;
			case DirectionLeft: pos = QPoint(mNextWidget->width() + 50, 0); break;
			case DirectionRight: pos = QPoint(-mNextWidget->width() - 50, 0); break;
		}

		mNextWidget->move(pos);
		mAnimationIn.setStartValue(pos);
		mAnimationIn.setEndValue(QPoint(0, 0));

		//Костыль от мерцания:
		QWidget *w = currentWidget();
		QStackedWidget::setCurrentWidget(mNextWidget);	//Смена текущего виджета на новый и обратно сделана для ресайза.
		QStackedWidget::setCurrentWidget(w);
		currentWidget()->hide();
		mNextWidget->show();

		mAnimationIn.start();
	}
}

void CAnimatedStackedWidget::secondAnimationFinished()
{
	if(mNextWidget)
	{
		QStackedWidget::setCurrentWidget(mNextWidget);
		mNextWidget = 0;
	}
}
