#ifndef CANIMATEDSTACKEDWIDGET_H
#define CANIMATEDSTACKEDWIDGET_H

#include <QtGui>

/**
StackedWidget с анимированным переходом.
Есть возможность задавать направление анимации перехода через параметер direction. Всего направлений четыре: вверх, вниз, влево, вправо. Также можно задать автоматический выбор направления, который зависит от соотношения индексов текущего и нового виджета.

\note Предполагается что виджеты не смещены относительно начала координат.

\author Demonist
*/
class CAnimatedStackedWidget : public QStackedWidget
{
    Q_OBJECT

	//types:
public:
	//! Направление анимации.
	enum Direction
	{
		DirectionUp,	//! Анимация смещения виджета вверх.
		DirectionDown,	//! Анимация смещения виджета вниз.
		DirectionLeft,	//! Анимация смещения виджета влево.
		DirectionRight	//! Анимация смещения виджета вправо.
	};

	//! Направление вертикальной анимации.
	enum VerticalDirection
	{
		VerticalAuto,			//! Выбирается автоматически в зависимости от индекса. Если новый индекс больше текущего, то будет выбрано VerticalUp; иначе - VerticalDown.
		VerticalAutoReverse,	//! Выбирается автоматически в зависимости от индекса. Если новый индекс больше текущего, то будет выбрано VerticalDown; иначе - VerticalUp.
		VerticalUp,				//! Анимация смещения виджета вверх.
		VerticalDown			//! Анимация смещения виджета вниз.
	};

	//! Направление горизонтальной анимации.
	enum HorizontalDirection
	{
		HorizontalAuto,			//! Выбирается автоматически в зависимости от индекса. Если новый индекс больше текущего, то будет выбрано HorizontalLeft; иначе - HorizontalRight.
		HorizontalAutoReverse,	//! Выбирается автоматически в зависимости от индекса. Если новый индекс больше текущего, то будет выбрано HorizontalRight; иначе - HorizontalLeft.
		HorizontalLeft,			//! Анимация смещения виджета влево.
		HorizontalRight			//! Анимация смещения виджета вправо.
	};

	//members:
private:
	QPropertyAnimation mAnimationOut, mAnimationIn;
	QWidget *mNextWidget;
	Direction mDirection;

	//methods:
public:
    explicit CAnimatedStackedWidget(QWidget *parent = 0);
	~CAnimatedStackedWidget();

public slots:
	void setCurrentWidgetAnimated(QWidget *widget, const Direction direction, const int duration = 500);
	inline void setCurrentIndexAnimated(const int index, const Direction direction, const int duration = 500);

	void setCurrentWidgetAnimatedVertical(QWidget *widget, const VerticalDirection direction = VerticalAuto, const int duration = 500);
	void setCurrentWidgetAnimatedHorizontal(QWidget *widget, const HorizontalDirection direction = HorizontalAuto, const int duration = 500);

	inline void setCurrentIndexAnimatedVertical(const int index, const VerticalDirection direction = VerticalAuto, const int duration = 500);
	inline void setCurrentIndexAnimatedHorizontal(const int index, const HorizontalDirection direction = HorizontalAuto, const int duration = 500);

	inline void setCurrentWidgetAnimatedVertical(QWidget *widget, const int duration);
	inline void setCurrentWidgetAnimatedHorizontal(QWidget *widget, const int duration);

	inline void setCurrentIndexAnimatedVertical(const int index, const int duration);
	inline void setCurrentIndexAnimatedHorizontal(const int index, const int duration);

	void slideVerticalNext(const int duration = 500);
	void slideVerticalPrev(const int duration = 500);
	void slideHorizontalNext(const int duration = 500);
	void slideHorizontalPrev(const int duration = 500);

private slots:
	void firstAnimationFinished();
	void secondAnimationFinished();
};

//inline implementation:

inline void CAnimatedStackedWidget::setCurrentIndexAnimated(const int index, const Direction direction, const int duration)
{
	setCurrentWidgetAnimated(widget(index), direction, duration);
}

inline void CAnimatedStackedWidget::setCurrentIndexAnimatedVertical(const int index, const VerticalDirection direction, const int duration)
{
	setCurrentWidgetAnimatedVertical(widget(index), direction, duration);
}

inline void CAnimatedStackedWidget::setCurrentIndexAnimatedHorizontal(const int index, const HorizontalDirection direction, const int duration)
{
	setCurrentWidgetAnimatedHorizontal(widget(index), direction, duration);
}

inline void CAnimatedStackedWidget::setCurrentWidgetAnimatedVertical(QWidget *widget, const int duration)
{
	setCurrentWidgetAnimatedVertical(widget, VerticalAuto, duration);
}

inline void CAnimatedStackedWidget::setCurrentWidgetAnimatedHorizontal(QWidget *widget, const int duration)
{
	setCurrentWidgetAnimatedHorizontal(widget, HorizontalAuto, duration);
}

inline void CAnimatedStackedWidget::setCurrentIndexAnimatedVertical(const int index, const int duration)
{
	setCurrentWidgetAnimatedVertical(widget(index), duration);
}

inline void CAnimatedStackedWidget::setCurrentIndexAnimatedHorizontal(const int index, const int duration)
{
	setCurrentWidgetAnimatedHorizontal(widget(index), duration);
}

#endif // CANIMATEDSTACKEDWIDGET_H
