#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>

namespace Global
{

enum ActionState{
	ActionActive = 1,		//! Мероприятие активно и еще не завершено.
	ActionInactive = 0,		//! Мероприятие не активно.
	ActionCanceled = 30,	//! Мероприятие отменено.
	ActionMoved = 20,		//! Мероприятние перенесено.
	ActionComplited = 100	//! Мероприятие завершено.
};
QString actionStateToText(const int actionState);
inline QString actionStateToText(const ActionState actionState);
int actionStateFromText(const QString &actionStateText);
QList<QString> actionStates();

enum SeatState{
	SeatFree = 100,		//! Место свободно.
	SeatSolded = 20,	//! Место занято.
	SeatReserved = 21,	//! Место забронировано.
	SeatNotAvaible = 1,	//! Место не доступно.
	SeatHided = 0		//! Место скрыто.
};
QString seatStateToText(const int seatState);
inline QString seatStateToText(const SeatState seatState);
int seatStateFromText(const QString &seatStateText);
QList<QString> seatStates();

//inline implementation:
inline QString actionStateToText(const ActionState actionState)
{
	return actionStateToText((const int)actionState);
}

inline QString seatStateToText(const SeatState seatState)
{
	return seatStateToText((const int)seatState);
}
}

#endif // GLOBAL_H