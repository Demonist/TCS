#ifndef CACTIONSEATITEM_H
#define CACTIONSEATITEM_H

#include "cseatitem.h"
#include "global.h"

class CActionSeatItem : public CSeatItem
{
	Q_OBJECT

private:
	QPropertyAnimation _mBlinkAnimationBlur;
	QPropertyAnimation _mBlinkAnimationUnblur;

	//members:
protected:
	Global::SeatState mSeatState;

	QSequentialAnimationGroup mBlinkAnimation;
	QParallelAnimationGroup mStateChangeAnimation;

	Global::SeatState mInitSeatState;

	//methods:
protected:
	void init();
	QColor brushForSeat() const;

public:
	explicit CActionSeatItem(const QString &connectionName, const int seatId);
	explicit CActionSeatItem(const QString &connectionName, const int seatId, const Global::SeatState seatState);
	~CActionSeatItem();

	void setBlinkAnimated(const qreal maximuxBlurRadius, const int durationMs = 300);
	void setSeatStateAnimated(const Global::SeatState seatState, const int durationMs = 300);

	bool needSave();

	inline Global::SeatState seatState() const;
	inline void setSeatState(const Global::SeatState seatState);

	static const char* itemName();
};

inline Global::SeatState CActionSeatItem::seatState() const
{
	return mSeatState;
}

inline void CActionSeatItem::setSeatState(const Global::SeatState seatState)
{
	mSeatState = seatState;
	mBrush = brushForSeat();
}

#endif // CACTIONSEATITEM_H

