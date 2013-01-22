#include "cseatitem.h"

#define SIZE 30
#define HALFSIZE SIZE/2

//protected:

void CSeatItem::init()
{
	setZValue(5.0);
	setData(0, CSeatItem::itemName());
	mBlurEffect.setBlurRadius(0.0);
	setGraphicsEffect(&mBlurEffect);

	mId = 0;
	animationInit();

	mHovered = false;
	mSelected = false;

	mBorderColor = QColor(70, 70, 70);
	mPenWidth = 4;
	mBrush = QColor(200, 200, 200);
}

void CSeatItem::animationInit()
{
	mScaleAnimation.setTargetObject(this);
	mScaleAnimation.setPropertyName("scale");
	mScaleAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mBrushAnimation.setTargetObject(this);
	mBrushAnimation.setPropertyName("brush");
	mBrushAnimation.setEasingCurve(QEasingCurve::InOutSine);
	connect(&mBrushAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(update_slot()));

	mBorderColorAnimation.setTargetObject(this);
	mBorderColorAnimation.setPropertyName("borderColor");
	mBorderColorAnimation.setEasingCurve(QEasingCurve::InOutSine);
	connect(&mBorderColorAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(update_slot()));

	mOpacityAnimation.setTargetObject(this);
	mOpacityAnimation.setPropertyName("opacity");
	mOpacityAnimation.setEasingCurve(QEasingCurve::InOutSine);

	mPenWidthAnimation.setTargetObject(this);
	mPenWidthAnimation.setPropertyName("penWidth");
	mPenWidthAnimation.setEasingCurve(QEasingCurve::InOutSine);
	connect(&mPenWidthAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(update_slot()));

	mBlurAnimation.setTargetObject(this);
	mBlurAnimation.setPropertyName("blurRadius");
	mBlurAnimation.setEasingCurve(QEasingCurve::InOutSine);
}

int CSeatItem::fontSizeForText(const QString &text) const
{
	//Для SIZE == 30:
	switch(text.size())
	{
	case 0:
	case 1:
	case 2:
		return 15;
	case 3:
		return 11;
	case 4:
		return 8;
	case 5:
		return 5;
	case 7:
	default:
		return 4;
	}
}

//public:

CSeatItem::CSeatItem() :
	QGraphicsObject()
{
	init();
	animationInit();
}

CSeatItem::CSeatItem(const QString &connectionName, const int seatId) :
	QGraphicsObject()
{
	init();
	animationInit();

	mId = seatId;

	QSqlQuery query(QSqlDatabase::database(connectionName));
	query.prepare("SELECT seatNumber, row, x, y FROM PlaceSchemes WHERE id = :id;");
	query.bindValue(":id", seatId);
	if(query.exec() && query.first())
	{
		mText = query.value(0).toString();
		mInitText = mText;
		mRow = query.value(1).toString();
		mInitRow = mRow;
		setPos(query.value(2).toReal(), query.value(3).toReal());
		mInitPoint = pos().toPoint();
	}
}

CSeatItem::~CSeatItem()
{

}

QRectF CSeatItem::boundingRect() const
{
	int halfPenWidth = mPenWidth;
	return QRectF(-HALFSIZE - halfPenWidth/2, -HALFSIZE - halfPenWidth/2, SIZE + mPenWidth, SIZE + mPenWidth);
}

void CSeatItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen pen;
	pen.setColor(mBorderColor);
	pen.setWidth(mPenWidth);

	painter->save();
	painter->setPen(pen);
	painter->setBrush(mBrush);
	painter->drawRoundedRect(-HALFSIZE, -HALFSIZE, SIZE, SIZE, 10, 10);
	QFont font = painter->font();
	if(isSelected())
		font.setBold(true);
	else
		font.setBold(false);
	font.setPointSize(fontSizeForText(mText));
	painter->setFont(font);
	painter->setPen(QColor(0, 0, 0));
	painter->drawText(-HALFSIZE, -HALFSIZE, SIZE, SIZE, Qt::AlignCenter, mText);
	painter->restore();
}

bool CSeatItem::needSave() const
{
	if(mId == 0
			|| mText != mInitText
			|| mRow != mInitRow
			|| pos().toPoint() != mInitPoint)
		return true;
	return false;
}

void CSeatItem::setScaleAnimated(const qreal newScale, const int durationMs)
{
	mScaleAnimation.stop();
	mScaleAnimation.setStartValue(scale());
	mScaleAnimation.setEndValue(newScale);
	mScaleAnimation.setDuration(durationMs);
	mScaleAnimation.start();
}

void CSeatItem::setBrushAnimated(const QColor &newBrushColor, const int durationMs)
{
	mBrushAnimation.stop();
	mBrushAnimation.setStartValue(brush());
	mBrushAnimation.setEndValue(newBrushColor);
	mBrushAnimation.setDuration(durationMs);
	mBrushAnimation.start();
}

void CSeatItem::setBorderColorAnimated(const QColor &newBorderColor, const int durationMs)
{
	mBorderColorAnimation.stop();
	mBorderColorAnimation.setStartValue(borderColor());
	mBorderColorAnimation.setEndValue(newBorderColor);
	mBorderColorAnimation.setDuration(durationMs);
	mBorderColorAnimation.start();
}

void CSeatItem::setOpacityAnimated(const qreal newOpacity, const int durationMs)
{
	mOpacityAnimation.stop();
	mOpacityAnimation.setStartValue(opacity());
	mOpacityAnimation.setEndValue(newOpacity);
	mOpacityAnimation.setDuration(durationMs);
	mOpacityAnimation.start();
}

void CSeatItem::setPenWidthAnimated(const int newPenWidth, const int durationMs)
{
	mPenWidthAnimation.stop();
	mPenWidthAnimation.setStartValue(penWidth());
	mPenWidthAnimation.setEndValue(newPenWidth);
	mPenWidthAnimation.setDuration(durationMs);
	mPenWidthAnimation.start();
}

void CSeatItem::setBlurAnimated(const qreal newBlurRadius, const int durationMs)
{
	mBlurAnimation.stop();
	mBlurAnimation.setStartValue(blurRadius());
	mBlurAnimation.setEndValue(newBlurRadius);
	mBlurAnimation.setDuration(durationMs);
	mBlurAnimation.start();
}

void CSeatItem::setHovered(const bool hovered)
{
	if(mHovered != hovered)
	{
		mHovered = hovered;
		if(isSelected() == false)
		{
			if(mHovered)
				setScale(1.1);
			else
				setScale(1.0);
		}
	}
}

void CSeatItem::setHoveredAnimated(const bool hovered, const int durationMs)
{
	if(mHovered != hovered)
	{
		mHovered = hovered;
		if(isSelected() == false)
		{
			if(mHovered)
				setScaleAnimated(1.1, durationMs);
			else
				setScaleAnimated(1.0, durationMs);
		}
	}
}

void CSeatItem::setSelected(const bool selected)
{
	if(mSelected != selected)
	{
		mSelected = selected;
		if(mSelected)
		{
			setPenWidth(5);
			if(isHovered() == false)
				setScale(1.1);
		}
		else
		{
			setPenWidth(4);
			if(isHovered() == false)
				setScale(1.0);
		}
	}
}

void CSeatItem::setSelectedAnimated(const bool selected, const int durationMs)
{
	if(mSelected != selected)
	{
		mSelected = selected;
		if(mSelected)
		{
			setPenWidth(5);
			if(isHovered() == false)
				setScaleAnimated(1.1, durationMs);
		}
		else
		{
			setPenWidth(4);
			if(isHovered() == false)
				setScaleAnimated(1.0, durationMs);
		}
	}
}

void CSeatItem::hideAnimated(const int durationMs)
{
	setOpacity(1.0);
	setBlurRadius(0.0);

	mOpacityAnimation.setStartValue(1.0);
	mOpacityAnimation.setEndValue(0.0);
	mOpacityAnimation.setDuration(durationMs);
	mBlurAnimation.setStartValue(1.0);
	mBlurAnimation.setEndValue(10.0);
	mBlurAnimation.setDuration(durationMs);

	QParallelAnimationGroup anim;
	anim.addAnimation(&mOpacityAnimation);
	anim.addAnimation(&mBlurAnimation);
	anim.start();

	//Цикл ожидания завершения анимации. Необходим чтобы скрыть элемент после анимации.
	while(anim.state() == QAbstractAnimation::Running)
	{
		CWaitThread::msleep(10);
		QApplication::processEvents();
	}
	hide();

	//Требуется удаление анимаций из группы т.к. anim в своем деструекторе попытается их удалить, что приведет к краху.
	anim.removeAnimation(&mOpacityAnimation);
	anim.removeAnimation(&mBlurAnimation);
}

void CSeatItem::showAnimated(const int durationMs)
{
	setOpacity(0.0);

	show();

	mOpacityAnimation.stop();
	mOpacityAnimation.setStartValue(opacity());
	mOpacityAnimation.setEndValue(1.0);
	mOpacityAnimation.setDuration(durationMs);
	mOpacityAnimation.start();
}

//static:

const char* CSeatItem::itemName()
{
	return "seatItem";
}
