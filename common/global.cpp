#include "global.h"

#define CRYPTED_KEY 0x6B

namespace Global
{

QString actionStateToText(const int actionState)
{
	switch(actionState)
	{
	case ActionInactive: return QObject::tr("Неактивен");
	case ActionActive: return QObject::tr("Активен");
	case ActionCanceled: return QObject::tr("Отменен");
	case ActionComplited: return QObject::tr("Завершен");
	default: return QObject::tr("Error");
	}
}

int actionStateFromText(const QString &actionStateText)
{
	if(actionStateText == actionStateToText(ActionInactive))
		return ActionInactive;
	if(actionStateText == actionStateToText(ActionActive))
		return ActionActive;
	if(actionStateText == actionStateToText(ActionCanceled))
		return ActionCanceled;
	if(actionStateText == actionStateToText(ActionComplited))
		return ActionComplited;
	return ActionInactive;
}

QList<QString> actionStates()
{
	QList<QString> states;
	states.append(actionStateToText(ActionActive));
	states.append(actionStateToText(ActionInactive));
	states.append(actionStateToText(ActionCanceled));
	states.append(actionStateToText(ActionComplited));
	return states;
}

QString seatStateToText(const int seatState)
{
	switch(seatState)
	{
	case SeatFree: return QObject::tr("Свободно");
	case SeatSolded: return QObject::tr("Занято");
	case SeatReserved: return QObject::tr("Забронировано");
	case SeatNotAvaible: return QObject::tr("Не доступно");
	case SeatHided: return QObject::tr("Скрыто");
	default: return QObject::tr("Error");
	}
}

int seatStateFromText(const QString &seatStateText)
{
	if(seatStateText == seatStateToText(SeatFree))
		return SeatFree;
	else if(seatStateText == seatStateToText(SeatSolded))
		return SeatSolded;
	else if(seatStateText == seatStateToText(SeatReserved))
		return SeatReserved;
	else if(seatStateText == seatStateToText(SeatNotAvaible))
		return SeatNotAvaible;
	else if(seatStateText == seatStateToText(SeatHided))
		return SeatHided;
	return SeatNotAvaible;
}

QList<QString> seatStates()
{
	QList<QString> states;
	states.append(seatStateToText(SeatFree));
	states.append(seatStateToText(SeatSolded));
	states.append(seatStateToText(SeatReserved));
	states.append(seatStateToText(SeatNotAvaible));
	states.append(seatStateToText(SeatHided));
	return states;
}

/**
Возвращает путь к каталогу, где располагается исполняемы файл.
Не зависит от директории запуска.
Путь возвращается \b без слеша в конце.
*/
QString currentPath()
{
	QDir path(QCoreApplication::arguments().value(0));
	path.cdUp();
	return path.absolutePath();
}

QByteArray crypt(const QString &data)
{
	QByteArray _data = data.toLocal8Bit();
	QByteArray crypted;
	for(int i = 0;  i < _data.size(); i++)
		crypted.append(((unsigned char)_data[i]) ^ CRYPTED_KEY);
	return crypted.toHex();
}

QString decrypt(const QByteArray &cryptedData)
{
	QByteArray crypted = QByteArray::fromHex(cryptedData);
	QByteArray decrypted;
	for(int i = 0; i < crypted.size(); i++)
		decrypted.append(((unsigned char)crypted[i]) ^ CRYPTED_KEY);
	return QObject::tr(decrypted.constData());
}

} //namespace Global
