#ifndef CACTIONITEM_H
#define CACTIONITEM_H

#include <QtGui>
#include <QtSql>
#include "cimages.h"

class CActionItem : public QGraphicsObject
{
	Q_OBJECT

private:
	QString mConnectionName;

	QPixmap mLogo;
	QString mTitle;
	QString mDate;

public:
	explicit CActionItem(const QString connectionName, const int actionId);
	
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
	
public slots:
	
};

#endif // CACTIONITEM_H
