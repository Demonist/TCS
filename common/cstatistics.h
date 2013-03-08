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
		TypeClientRegistered = 1
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

class CStatisticClientRegisteredType : public _CStaticticClientTypePrivate
{
public:
	QString clientName;
	QString clientBirthDate;
	QString clientLogin;
	QString clientPassword;
	QString clientPhone;

protected:
	virtual void dataToStream(QDataStream &stream) const
	{
		if(isValid())
		{
			_CStaticticClientTypePrivate::dataToStream(stream);
			stream << clientName << clientBirthDate << clientLogin << clientPassword << clientPhone;
		}
	}

	virtual void dataFromStream(QDataStream &stream)
	{
		_CStaticticClientTypePrivate::dataFromStream(stream);
		stream >> clientName >> clientBirthDate >> clientLogin >> clientPassword >> clientPhone;
	}

public:
	CStatisticClientRegisteredType() :
		_CStaticticClientTypePrivate()
	{
		mType = CStatistics::TypeClientRegistered;
	}

	virtual bool isValid() const
	{
		return _CStaticticClientTypePrivate::isValid()
				&&clientName.isEmpty() == false
				&& clientLogin.isEmpty() == false
				&& clientPassword.isEmpty() == false;
	}
};

#endif // CSTATISTICS_H
