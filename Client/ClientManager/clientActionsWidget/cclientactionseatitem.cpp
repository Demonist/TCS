#include "cclientactionseatitem.h"

//private:

void CClientActionSeatItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	if(mState == Global::SeatFree)
		setHoveredAnimated(true, 500);
}

void CClientActionSeatItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	if(mState == Global::SeatFree)
		setHoveredAnimated(false, 500);

}

void CClientActionSeatItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	CSeatItem::mousePressEvent(event);
	event->accept();
}

void CClientActionSeatItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	CSeatItem::mouseReleaseEvent(event);
	if(mState == Global::SeatFree && event->button() == Qt::LeftButton)
	{
		if(mSelected)
			setPenWidth(2);
		else
			setPenWidth(4);
		mSelected = !mSelected;
		emit selectionChanged(this, mSelected);
	}
}

//public:

CClientActionSeatItem::CClientActionSeatItem(const QString &connectionName, const int id) :
	CSeatItem()
{
	mConnectionName = connectionName;
	mState = Global::SeatHided;
	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);
	setData(0, itemName());
	setPenWidth(2);
	mId = id;
	mPrice = 0;
	mPenalty = 0;
}

void CClientActionSeatItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	CSeatItem::paint(painter, option, widget);

	if(mState == Global::SeatReserved)
	{
		painter->save();

		painter->setOpacity(0.5f);

		QRect r = boundingRect().toRect();
		r.adjust(-2, -2, 2, 2);

		painter->setPen(Qt::NoPen);
		painter->setBrush(QColor(230, 230, 230));
		painter->drawRoundedRect(r, 10, 10);

		painter->setOpacity(1.0f);
		r.adjust(7, 7, -7, -7);
		const static QPixmap mReserved(":/clientImages/shield.png");
		painter->drawPixmap(r, mReserved);

		painter->restore();
	}
}

void CClientActionSeatItem::setSeatState(const Global::SeatState state)
{
	if(mState != state)
	{
		mState = state;
		updateForSeat();
	}
}

void CClientActionSeatItem::updateForSeat()
{
	if(mState != Global::SeatFree)
	{
		setPenWidth(2);
		setPenColor(QColor(150, 150, 150));

		if(mState != Global::SeatReserved)
			setBrushColor(QColor(230, 230, 230));
	}
}

void CClientActionSeatItem::updateData()
{

}

const char* CClientActionSeatItem::itemName()
{
	return "clientActionItem";
}
