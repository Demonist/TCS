#ifndef CACTIONSEATITEM_H
#define CACTIONSEATITEM_H

#include "cseatitem.h"
#include "global.h"

class CActionSeatItem : public CSeatItem
{
	Q_OBJECT

	//members:
protected:
	Global::SeatState mSeatState;
	int mPriceGroupId;

	Global::SeatState mInitSeatState;
	int mInitPriceGroupId;

	//methods:
protected:
	void init();
	QColor brushForSeat() const;

public:
	explicit CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y, const Global::SeatState seatState, const int priceGroupId, const QString &priceColor);
	explicit CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y, const Global::SeatState seatState);
	explicit CActionSeatItem(const int seatId, const QString &seatText, const QString &row, const qreal &x, const qreal &y);
	~CActionSeatItem();

	void setSeatStateAnimated(const Global::SeatState seatState, const int durationMs = 300);

	bool needSave();

	inline Global::SeatState seatState() const;
	inline void setSeatState(const Global::SeatState seatState);

	inline int priceGroupId() const;
	inline void setPriceGroupId(const int id);

	static const char* itemName();
};

inline Global::SeatState CActionSeatItem::seatState() const
{
	return mSeatState;
}
inline void CActionSeatItem::setSeatState(const Global::SeatState seatState)
{
	mSeatState = seatState;
	setBrushColor(brushForSeat());
}

inline int CActionSeatItem::priceGroupId() const
{
	return mPriceGroupId;
}
inline void CActionSeatItem::setPriceGroupId(const int id)
{
	mPriceGroupId = id;
}

#endif // CACTIONSEATITEM_H

