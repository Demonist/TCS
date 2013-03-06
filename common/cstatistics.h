#ifndef CSTATISTICS_H
#define CSTATISTICS_H

#include <QtCore>
#include <QtSql>

class CAbstractStatisticType;

/**
Класс для работы со статистикой в базе данных.
Данный класс выполнен в виде синглетона.

Позволяет добавлять и получать статистику из базы данных. Статистические типы могут быть произвольными, что позволяет использовать данный класс для ведения логов.
Работа со статистикой ведется через статистические типы, которые реализуются через наследников класса \class CAbstractStatisticType.

@author Demonist
*/
class CStatistics
{
public:
	enum Type{
		TypeUnknown = 0,
		TypeTicketSolded = 1,
		TypeTicketReturned = 2,
		TypeClientRegistered = 3
	};

private:
	CStatistics();
	CStatistics(const CStatistics &obj);
	CStatistics& operator = (const CStatistics &obj);

protected:
	QString mConnectionName;

public:
	static CStatistics* instance();
	static CStatistics* instance(const QString &connectionName);
	~CStatistics();

	static CAbstractStatisticType* fromByteArray(const QByteArray &data);

	void write(const CAbstractStatisticType &type, const QString &text = QString());
};

//***************************************************************************//

/**
Интерфейс для создания нового типа статистики.
*/
class CAbstractStatisticType
{
protected:
	CStatistics::Type mType;

protected:
	virtual void dataToStream(QDataStream &stream) const = 0;
	virtual void dataFromStream(QDataStream &stream) = 0;

public:
	CAbstractStatisticType()
	{
		mType = CStatistics::TypeUnknown;
	}

	virtual bool isValid() const = 0;

	QByteArray toByteArray() const
	{
		QByteArray ret;
		QDataStream stream(&ret, QIODevice::WriteOnly);
		stream << (int) mType;
		dataToStream(stream);
		if(ret.size() >= 65535)	//Ограничение на размер BLOB'а.
			ret.clear();
		return ret;
	}

	bool fromStream(QDataStream &stream)
	{
		dataFromStream(stream);
		return isValid();
	}
	inline bool fromByteArray(const QByteArray &data)
	{
		QDataStream stream(data);
		return fromStream(stream);
	}

	inline CStatistics::Type type() const
	{
		return mType;
	}
};

/**
Интерфейс для создания статистики для клиента.
*/
class _CStaticticClientTypePrivate : public CAbstractStatisticType
{
public:
	int sellerId;
	int marketId;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		if(isValid())
			stream << sellerId << marketId;
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		stream >> sellerId >> marketId;
	}

public:
	_CStaticticClientTypePrivate()
	{
		mType = CStatistics::TypeUnknown;
		sellerId = 0;
		marketId = 0;
	}

	virtual bool isValid() const
	{
		return sellerId && marketId;
	}
};

/**
Интерфейс для создания статистики работы с билетами.
*/
class _CStatisticTicketTypePrivate : public _CStaticticClientTypePrivate
{
public:
	QString barCode;
	QString ticketIdentifier;
	int clientId;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		if(isValid())
		{
			_CStaticticClientTypePrivate::dataToStream(stream);
			stream << barCode << ticketIdentifier << clientId;
		}
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		_CStaticticClientTypePrivate::dataFromStream(stream);
		stream >> barCode >> ticketIdentifier >> clientId;
	}

public:
	_CStatisticTicketTypePrivate()
	{
		mType = CStatistics::TypeUnknown;
		clientId = 0;
	}

	virtual bool isValid() const
	{
		return _CStaticticClientTypePrivate::isValid()
				&& barCode.isEmpty() == false
				&& ticketIdentifier.isEmpty() == false;
	}
};

class CStatisticTicketSoldedType : public _CStatisticTicketTypePrivate
{
public:
	int actionId;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		_CStatisticTicketTypePrivate::dataToStream(stream);
		stream << actionId;
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		_CStatisticTicketTypePrivate::dataFromStream(stream);
		{
			stream >> actionId;
		}
	}

public:
	CStatisticTicketSoldedType() :
		_CStatisticTicketTypePrivate()
	{
		mType = CStatistics::TypeTicketSolded;
		actionId = 0;
	}

	virtual bool isValid() const
	{
		return _CStatisticTicketTypePrivate::isValid() && actionId;
	}
};

class CStatisticTicketReturnedType : public CStatisticTicketSoldedType
{
public:
	int ticketId;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		if(isValid())
		{
			CStatisticTicketSoldedType::dataToStream(stream);
			stream << ticketId;
		}
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		CStatisticTicketSoldedType::dataFromStream(stream);
		stream >> ticketId;
	}

public:
	CStatisticTicketReturnedType() :
		CStatisticTicketSoldedType()
	{
		mType = CStatistics::TypeTicketReturned;
		ticketId = 0;
	}

	virtual bool isValid() const
	{
		return CStatisticTicketSoldedType::isValid() && ticketId;
	}
};

class CStatisticClientRegistered : public CAbstractStatisticType
{
public:
	QString clientName;
	QString clientLogin;
	QString clientPhone;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		if(isValid())
		{
			stream << clientName << clientLogin << clientPhone;
		}
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		stream >> clientName >> clientLogin >> clientPhone;
	}

public:
	CStatisticClientRegistered()
	{
		mType = CStatistics::TypeClientRegistered;
	}

	virtual bool isValid() const
	{
		return clientName.isEmpty() == false && clientLogin.isEmpty() == false;
	}
};

#endif // CSTATISTICS_H
