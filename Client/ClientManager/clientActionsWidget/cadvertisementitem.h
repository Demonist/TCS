#ifndef CADVERTISEMENTITEM_H
#define CADVERTISEMENTITEM_H

#include "cabstractgraphicsitem.h"

class CAdvertisementItem : public CAbstractGraphicsItem
{
	Q_OBJECT

protected:
	int mId;
	QPixmap mPixmap;

public:
	explicit CAdvertisementItem();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	inline int id() const;
	inline QPixmap pixmap() const;

public slots:
	inline void setId(const int id);
	inline void setPixmap(const QPixmap &pixmap);
};

//inline implementation:

inline int CAdvertisementItem::id() const
{
	return mId;
}
inline void CAdvertisementItem::setId(const int id)
{
	mId = id;
}

inline QPixmap CAdvertisementItem::pixmap() const
{
	return mPixmap;
}
inline void CAdvertisementItem::setPixmap(const QPixmap &pixmap)
{
	mPixmap = pixmap;
	update();
}

#endif // CADVERTISEMENTITEM_H
