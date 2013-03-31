#include "ccachechecker.h"

//Перечень таблиц для которых реализовано кеширование:
const static QStringList cachedTables = QObject::tr("Users Admins Categories Places PlaceSchemes Clients Actions ActionScheme ActionPriceGroups Markets Tickets ReturnedTickets Reservations Statistics ComplitedActions Advertisements").split(' ', QString::SkipEmptyParts);

//protected:

void CCacheChecker::initialize(const QStringList &tables, const QString &connectionName)
{
	mTables = static_cast<QList<QString> >(tables);
	mConnectionName = connectionName;

	for(int i = 0; i < mTables.size(); i++)
		if(cachedTables.contains(mTables[i]) == false)
			qDebug("CCacheChecker::CCacheChecker: Wrong table `%s` to caching.", qPrintable(mTables[i]));
}

//public:
/**
\param tables содержит перечень таблиц, разделенных пробелом или запятой с пробелом.
\param connectionName имя подключения к базе данных.
*/
CCacheChecker::CCacheChecker(const QString &tables, const QString &connectionName)
{
	QString t = tables.trimmed();
	t.replace(QRegExp("\\S,\\S"), " ");
	t.remove(',');
	initialize(t.split(' '), connectionName);
}

/**
\param tables содержит список таблиц.
\param connectionName имя подключения к базе данных.
*/
CCacheChecker::CCacheChecker(const QStringList &tables, const QString &connectionName)
{
	initialize(tables, connectionName);
}

/**
  Функци япроверят нужно ли обновление кешированных данных для указанных в конструкторе таблиц.
*/
bool CCacheChecker::isNeedUpdate() const
{
	bool ret = true;
	bool  missTable = false;

	mLastCheck.clear();

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	if(query.exec("SELECT tableName, dateTime FROM Cache"))
	{
		QList<QString> tables = mTables;
		while(query.next())
		{
			int index = tables.indexOf(query.value(0).toString());
			if(index >= 0)
			{
				mLastCheck.insert(tables[index], query.value(1).toDateTime());
				tables.removeAt(index);
			}
		}
		if(tables.isEmpty() == false)
			missTable = true;
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	if(mTables.isEmpty())
		missTable = true;

	if(!missTable)	//Если все таблицы кешируемые.
		ret = !(mLastCheck == mLastUpdate);
	return ret;
}
