#include "cactionseatitem.h"

//protected:

void CActionSeatItem::init()
{
	setData(0, CActionSeatItem::itemName());

	_mBlinkAnimationBlur.setTargetObject(this);
	_mBlinkAnimationBlur.setPropertyName("blurRadius");
	_mBlinkAnimationBlur.setStartValue(0.0);
	_mBlinkAnimationBlur.setEndValue(1.0);

	_mBlinkAnimationUnblur.setTargetObject(this);
	_mBlinkAnimationUnblur.setPropertyName("blurRadius");
	_mBlinkAnimationUnblur.setStartValue(1.0);
	_mBlinkAnimationUnblur.setEndValue(0.0);

	mBlinkAnimation.addAnimation(&_mBlinkAnimationBlur);
	mBlinkAnimation.addPause(50);
	mBlinkAnimation.addAnimation(&_mBlinkAnimationUnblur);

	mStateChangeAnimation.addAnimation(&mBrushAnimation);
	mStateChangeAnimation.addAnimation(&mBlinkAnimation);

	mInitSeatState = mSeatState;
	mBrush = brushForSeat();
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
		return QColor(200, 200, 200);
	}
}

//public:

CActionSeatItem::CActionSeatItem(const QString &connectionName, const int seatId) :
	CSeatItem(connectionName, seatId)
{
	mSeatState = Global::SeatHided;

	init();
}

CActionSeatItem::CActionSeatItem(const QString &connectionName, const int seatId, const Global::SeatState seatState) :
	CSeatItem(connectionName, seatId)
{
	mSeatState = seatState;

	init();
}

CActionSeatItem::~CActionSeatItem()
{
	mBlinkAnimation.removeAnimation(&_mBlinkAnimationBlur);
	mBlinkAnimation.removeAnimation(&_mBlinkAnimationUnblur);

	mStateChangeAnimation.removeAnimation(&mBrushAnimation);
	mStateChangeAnimation.removeAnimation(&mBlinkAnimation);
}

void CActionSeatItem::setBlinkAnimated(const qreal maximuxBlurRadius, const int durationMs)
{
	mBlinkAnimation.stop();
	_mBlinkAnimationBlur.setEndValue(maximuxBlurRadius);
	_mBlinkAnimationBlur.setDuration(durationMs / 2);
	_mBlinkAnimationUnblur.setStartValue(maximuxBlurRadius);
	_mBlinkAnimationUnblur.setDuration(durationMs / 2);
	mBlinkAnimation.start();
}

void CActionSeatItem::setSeatStateAnimated(const Global::SeatState seatState, const int durationMs)
{
	if(mSeatState != seatState)
	{
		mSeatState = seatState;

		mStateChangeAnimation.stop();

		mBrushAnimation.setStartValue(mBrush);
		mBrushAnimation.setEndValue(brushForSeat());
		mBrushAnimation.setDuration(durationMs);

		_mBlinkAnimationBlur.setEndValue(1.2);
		_mBlinkAnimationBlur.setDuration(durationMs / 2);
		_mBlinkAnimationUnblur.setStartValue(1.2);
		_mBlinkAnimationUnblur.setDuration(durationMs / 2);

		mStateChangeAnimation.start();
	}
}

bool CActionSeatItem::needSave()
{
	if(mInitSeatState != mSeatState)
		return true;
	return CSeatItem::needSave();
}

//static:

const char* CActionSeatItem::itemName()
{
	return "actionSeatItem";
}

