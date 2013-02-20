#ifndef CCLIENTACTIONSEATITEM_H
#define CCLIENTACTIONSEATITEM_H

#include "cseatitem.h"
#include "global.h"

class CClientActionSeatItem : public CSeatItem
{
	Q_OBJECT

private:
	Global::SeatState mState;
	int mPrice;

private:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
	explicit CClientActionSeatItem(const QString &connectionName, const int id, const int price);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	inline int price() const;
	
signals:
	void selectionChanged(CClientActionSeatItem *item, bool isSelected);

public slots:
	void setSeatState(const Global::SeatState state);
	void updateForSeat();
	void updateData();

	static const char* itemName();
};

//inline implementation:

inline int CClientActionSeatItem::price() const
{
	return mPrice;
}

#endif // CCLIENTACTIONSEATITEM_H
