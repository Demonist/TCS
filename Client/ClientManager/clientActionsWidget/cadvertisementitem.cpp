#include "cadvertisementitem.h"

CAdvertisementItem::CAdvertisementItem() :
	CAbstractGraphicsItem()
{
	mId = 0;
}

QRectF CAdvertisementItem::boundingRect() const
{
	QRectF rect = static_cast<QRectF>(mPixmap.rect());
	rect.moveCenter(QPointF(0.0f, 0.0f));
	return rect;
}

void CAdvertisementItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(painter)
	{
		painter->drawPixmap(boundingRect().toRect(), mPixmap);
	}
}
