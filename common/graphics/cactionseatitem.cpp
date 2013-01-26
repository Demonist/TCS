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

CActionSeatItem::CActionSeatItem(const QString &connectionName, const int seatId) :
	CSeatItem(connectionName, seatId)
{
	mSeatState = Global::SeatHided;
	mPriceGroupId = 0;
	init();
}

CActionSeatItem::CActionSeatItem(const QString &connectionName, const int seatId, const Global::SeatState seatState, const int priceGroupId) :
	CSeatItem(connectionName, seatId)
{
	mSeatState = seatState;
	mPriceGroupId = priceGroupId;
	init();

	QSqlQuery query(QSqlDatabase::database(connectionName));
	query.prepare("SELECT color FROM ActionPriceGroups WHERE id = :id;");
	query.bindValue(":id", mPriceGroupId);
	if(query.exec() && query.first())
		setPenColor(query.value(0).toString());
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

