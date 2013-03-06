#include "cstatistics.h"

static CStatistics *statistics = 0;

//private:

CStatistics::CStatistics()
{
	mConnectionName = "common";
}

CStatistics::CStatistics(const CStatistics &obj)
{

}

CStatistics& CStatistics::operator =(const CStatistics &obj)
{
	return *this;
}

//public:

CStatistics* CStatistics::instance()
{
	if(!statistics)
		statistics = new CStatistics();

	return statistics;
}

CStatistics* CStatistics::instance(const QString &connectionName)
{
	if(!statistics)
		statistics = new CStatistics();
	statistics->mConnectionName = connectionName;
	return statistics;
}

CStatistics::~CStatistics()
{
	statistics = 0;
}

CAbstractStatisticType* CStatistics::fromByteArray(const QByteArray &data)
{
	CAbstractStatisticType *ret = 0;
	QDataStream stream(data);
	int type;
	stream >> type;
	switch(type)
	{
		case TypeTicketSolded: ret = new CStatisticTicketSoldedType(); break;
		case TypeTicketReturned: ret = new CStatisticTicketReturnedType(); break;
		case TypeClientRegistered: ret = new CStatisticClientRegistered(); break;
	}

	if(ret)
	{
		if(false == ret->fromStream(stream))
		{
			delete ret;
			ret = 0;
		}
	}
	return ret;
}

void CStatistics::write(const CAbstractStatisticType &type, const QString &text)
{
	if(type.isValid() == false)
	{
		qCritical("CStatistics::write: type is not valid.");
		return;
	}

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("INSERT INTO Statistics VALUES(NULL, :type, :text, :data, NULL);");
	query.bindValue(":type", (int)type.type());
	query.bindValue(":text", text);
	query.bindValue(":data", type.toByteArray());
	if(!query.exec())
		qCritical(qPrintable(query.lastError().text()));
}
