#include "cactionitem.h"

//protected:

void CActionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	setScaleAnimated(1.05);
}

void CActionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setScaleAnimated(1.0);
}

//public:

CActionItem::CActionItem(const QString connectionName, const int actionId) :
	CAbstractGraphicsItem()
{
	setAcceptHoverEvents(true);
	mConnectionName = connectionName;

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT title, dateTime, id_logoImage FROM Actions WHERE id = :id;");
	query.bindValue(":id", actionId);
	if(query.exec() && query.first())
	{
		mTitle = query.value(0).toString();
		mDate = query.value(1).toDateTime().toString("dd.MM.yyyy hh:mm");
		mLogo = CImages::instance()->image(query.value(2).toInt());
		mLogo = mLogo.scaled(200, 150, Qt::KeepAspectRatio);
	}
}

QRectF CActionItem::boundingRect() const
{
	return QRectF(0.0f, 0.0f, 200.0f, 200.0f);
}

void CActionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->save();

	QPen pen;

	pen.setWidth(5);
	pen.setColor(qRgba(200, 200, 200, 50));
	painter->setPen(pen);
	painter->setBrush(QColor(240, 240, 240, 100));
	painter->drawRoundedRect(boundingRect(), 10, 10);

	painter->drawPixmap(0, 0, mLogo);
	painter->setBrush(Qt::NoBrush);
	painter->drawRoundedRect(boundingRect(), 10, 10);

	pen.setColor(qRgb(255, 255, 255));
	painter->setPen(pen);
	painter->drawText(5, 150, 190, 50, Qt::AlignHCenter | Qt::AlignVCenter, mTitle + '\n' + mDate);

	painter->restore();
}
