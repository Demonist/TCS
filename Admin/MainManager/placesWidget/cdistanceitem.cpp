#include "cdistanceitem.h"

CDistanceItem::CDistanceItem() :
	QGraphicsObject()
{
	setZValue(2.0);
	setData(0, CDistanceItem::itemName());
}

QRectF CDistanceItem::boundingRect() const
{
	QRect rect;
	if(mPos.x() < 0 || mPos.y() < 0)
	{
		rect.setTopLeft(mPos);
		rect.adjust(-15, -15, 15, 15);
	}
	else
	{
		rect.setBottomRight(mPos);
		rect.adjust(-15, -15, 15, 15);
	}
	return rect;
}

void CDistanceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();

	QPen pen;
	pen.setWidth(2);
	pen.setColor(qRgb(0, 255, 0));

	painter->setPen(pen);
	painter->setBrush(QColor(230, 255, 230));

	painter->drawLine(QPoint(0, 0), mPos);

	const QString dist = QString::number(distance());
	QRect textRect = painter->fontMetrics().boundingRect(dist);
	textRect.moveCenter(boundingRect().center().toPoint());
	textRect.adjust(-3, 1, 3, 1);
	painter->drawRoundRect(textRect, 50, 50);

	if(mBold)
	{
		QFont font = painter->font();
		font.setBold(mBold);
		painter->setFont(font);
	}
	pen.setColor(qRgb(0, 0, 0));
	painter->setPen(pen);
	painter->drawText(textRect, Qt::AlignCenter, dist);

	painter->restore();
}

const char* CDistanceItem::itemName()
{
	return "distanceItem";
}
