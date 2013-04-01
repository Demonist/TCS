#include "cactionseatitem.h"

//protected:

void CActionSeatItem::init()
{
	setData(0, CActionSeatItem::itemName());

	mBrushColor = brushForSeat();
	mInitSeatState = mSeatState;
	mInitPriceGroupId = mPriceGroupId;
}

QColor CActionSeatItem::brushForSeat() const
{
	switch(mSeatState)
	{
	case Global::SeatFree: return QColor(0, 200, 0);
	case Global::SeatSolded: return QColor(200, 0, 0);
	case Global::SeatNotAvaible: return QColor(150, 150, 150);
	case Global::SeatReserved: return QColor(200, 50, 50);
	case Global::SeatHided:
	default:
		return QColor(240, 240, 240);
	}
}

//public:

CActionSeatItem::CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y, const Global::SeatState seatState, const int priceGroupId, const QString &priceColor) :
	CSeatItem(seatId, seatText, row, x, y)
{
	mSeatState = seatState;
	mPriceGroupId = priceGroupId;
	setPenColor(priceColor);
	init();
}

CActionSeatItem::CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y, const Global::SeatState seatState) :
	CSeatItem(seatId, seatText, row, x, y)
{
	mSeatState = seatState;
	mPriceGroupId = 0;
	init();
}

CActionSeatItem::CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y) :
	CSeatItem(seatId, seatText, row, x, y)
{
	mSeatState = Global::SeatHided;
	mPriceGroupId = 0;
	init();
}

CActionSeatItem::~CActionSeatItem()
{

}

void CActionSeatItem::setSeatStateAnimated(const Global::SeatState seatState, const int durationMs)
{
	if(mSeatState != seatState)
	{
		mSeatState = seatState;

		setBrushColorAnimated(brushForSeat(), durationMs);
	}
}

bool CActionSeatItem::needSave()
{
	if(mInitSeatState != mSeatState
	   || mInitPriceGroupId != mPriceGroupId)
		return true;
	return CSeatItem::needSave();
}

//static:

const char* CActionSeatItem::itemName()
{
	return "actionSeatItem";
}

