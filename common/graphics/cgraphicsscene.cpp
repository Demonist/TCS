#include "cgraphicsscene.h"

//protected:

void CGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
	painter->save();

	if(mDrawBackground)
		painter->drawPixmap(mImageShear, mImage);

	QPen pen;
	pen.setColor(qRgb(100, 100, 100));
	pen.setWidth(5);

	painter->setPen(pen);
	painter->drawRoundedRect(sceneRect(), 10, 10);
	if(mDrawAxis)
	{
		pen.setColor(qRgb(200, 200, 200));
		pen.setWidth(2);
		painter->setPen(pen);
		int wh = width() / 2;
		int hh = height() / 2;
		painter->drawLine(wh, 0, wh, height());
		painter->drawLine(0, hh, width(), hh);

		pen.setColor(qRgb(230, 230, 230));
		pen.setWidth(1);
		painter->setPen(pen);
		wh /= 2;
		hh /= 2;
		painter->drawLine(wh, 0, wh, height());
		painter->drawLine(0, hh, width(), hh);
		wh *= 3;
		hh *= 3;
		painter->drawLine(wh, 0, wh, height());
		painter->drawLine(0, hh, width(), hh);
	}

	painter->restore();
}

//public:

CGraphicsScene::CGraphicsScene(QObject *parent) :
	QGraphicsScene(parent)
{
	mDrawBackground = true;
	mDrawAxis = false;
}
