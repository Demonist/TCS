#ifndef CDISTANCEITEM_H
#define CDISTANCEITEM_H

#include <QtGui>

class CDistanceItem : public QGraphicsObject
{
	Q_OBJECT

protected:
	QPoint mPos;
	bool mBold;

public:
	explicit CDistanceItem();

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	inline int distance() const;

	static const char* itemName();
	
public slots:
	inline void setTargetPos(const QPoint &pos);
	inline void setTargetPos(const QPointF &pos);
	inline void setBold(const bool bold);
};

//inline implementation:

inline void CDistanceItem::setTargetPos(const QPoint &pos)
{
	mPos = pos;
	update();
}
inline void CDistanceItem::setTargetPos(const QPointF &pos)
{
	mPos = pos.toPoint();
	update();
}

inline int CDistanceItem::distance() const
{
	return qSqrt(qPow(mPos.x(), 2) + qPow(mPos.y(), 2));
}

inline void CDistanceItem::setBold(const bool bold)
{
	mBold = bold;
	update();
}

#endif // CDISTANCEITEM_H
