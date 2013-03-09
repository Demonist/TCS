#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>

namespace Global
{

enum ActionState{
	ActionActive = 1,		//! Мероприятие активно и еще не завершено.
	ActionInactive = 0,		//! Мероприятие не активно.
	ActionCanceled = 30,	//! Мероприятие отменено.
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

QString currentPath();

QByteArray crypt(const QString &data);
QString decrypt(const QByteArray &cryptedData);

//inline implementation:

inline QString actionStateToText(const ActionState actionState)
{
	return actionStateToText((const int)actionState);
}

inline QString seatStateToText(const SeatState seatState)
{
	return seatStateToText((const int)seatState);
}

} //namespace Global

#endif // GLOBAL_H
