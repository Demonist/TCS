#include "cticketidentifier.h"

int slavePosition(const int masterValue)
{
	switch(masterValue)
	{
		case 1: return 6;
		case 2: return 5;
		case 3: return 9;
		case 4: return 3;
		case 5: return 11;
		case 6: return 4;
		case 7: return 7;
		case 8: return 8;
		case 9: return 10;
	}
	return masterValue;
}

//public:

CTicketIdentifier::CTicketIdentifier()
{
}

CTicketIdentifier::CTicketIdentifier(const QString &identifier)
{
	mIdentifier = identifier;
}

void CTicketIdentifier::render(const int w, const int h, QPainter *painter) const
{
	EAN13 ean13(mIdentifier);
	ean13.draw(QRectF(0.0f, 0.0f, w, h), painter);
}

/**
Функция возвращает данные для заданного идентификатора.
Возвращает \b не пустую строку в случае успеха.
*/
QString CTicketIdentifier::data() const
{
	QString data;

	if(isValid())
	{
		switch(mIdentifier.size())
		{
			case 12:
			case 13:
			{
				const char master = mIdentifier.mid(2, 1).toInt();
				const char slavePos = slavePosition(master);

				data = mIdentifier;
				if(data.size() == 13)
					data.chop(1);
				data.remove(2, 1);
				data.remove(slavePos - 1, 1);
			}
				break;
		}
	}

	return data;
}

//static:

/**
Функция генерирует идентификатор исходя из данных.
Успешность проверяется методом \a isValid у полученного идентификатора.
\param data содержит данные. Поддерживаются следующие форматы: размер 10 символов, только цифры.
*/
CTicketIdentifier CTicketIdentifier::generate(const QString &data)
{
	QString identifier;

	switch(data.size())
	{
		case 10:
		{
			bool conversion;
			data.toInt(&conversion, 10);
			if(conversion == true)
			{
				const char master = 1 + qrand() % 9;
				const char slave = 10 - master;

				identifier = data;

				identifier.insert(2, QString::number(master));
				identifier.insert(slavePosition(master), QString::number(slave));
			}
		}
			break;
	}

	return CTicketIdentifier(identifier);
}

/**
Функция генерирует идентификатор размером 12 символов.
*/
CTicketIdentifier CTicketIdentifier::generate()
{
	QString identifier = QString::number(qrand()%10000000000);
	while(identifier.size() < 10)
		identifier.prepend('0');
	const char master = 1 + qrand() % 9;
	const char slave = 10 - master;
	identifier.insert(2, QString::number(master));
	identifier.insert(slavePosition(master), QString::number(slave));

	return CTicketIdentifier(identifier);
}

bool CTicketIdentifier::isValidIdentifier(const QString &identifier)
{
	switch(identifier.size())
	{
		case 12:
		case 13:
		{
			bool conversion = false;
			identifier.toULongLong(&conversion);
			if(conversion == true)
			{
				const char master = identifier.mid(2, 1).toInt();
				return 10 - master == identifier.mid(slavePosition(master), 1).toInt();
			}
		}
			break;
	}

	return false;
}
