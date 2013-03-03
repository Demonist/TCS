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
	int mPenalty;
	QString mPriceGroupTitle;

private:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
	explicit CClientActionSeatItem(const QString &connectionName, const int id);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	inline int price() const;
	inline int penalty() const;
	inline QString priceGroupTitle() const;
	
signals:
	void selectionChanged(CClientActionSeatItem *item, bool isSelected);

public slots:
	void setSeatState(const Global::SeatState state);
	void updateForSeat();
	void updateData();
	inline void setPrice(const int price);
	inline void setPenalty(const int penalty);
	inline void setPriceGroupTitle(const QString &priceGroupTitle);

public:
	static const char* itemName();
};

//inline implementation:

inline int CClientActionSeatItem::price() const
{
	return mPrice;
}
inline void CClientActionSeatItem::setPrice(const int price)
{
	mPrice = price;
}

inline int CClientActionSeatItem::penalty() const
{
	return mPenalty;
}
inline void CClientActionSeatItem::setPenalty(const int penalty)
{
	mPenalty = penalty;
}

inline QString CClientActionSeatItem::priceGroupTitle() const
{
	return mPriceGroupTitle;
}
inline void CClientActionSeatItem::setPriceGroupTitle(const QString &priceGroupTitle)
{
	mPriceGroupTitle = priceGroupTitle;
}

#endif // CCLIENTACTIONSEATITEM_H
