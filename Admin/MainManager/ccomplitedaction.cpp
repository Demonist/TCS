#include "ccomplitedaction.h"

CComplitedAction::CComplitedAction()
{
	mValid = false;
	totalTicketsSolded = 0;
	ticketsReturned = 0;
	totalSumm = 0;
	penaltySumm = 0;
	soldedBySite = 0;
}

bool CComplitedAction::makeFromAction(const int actionId, const QString &connectionName)
{
	totalTicketsSolded = 0;
	ticketsReturned = 0;
	totalSumm = 0;
	penaltySumm = 0;
	soldedBySite = 0;
	soldedByMarkets.clear();
	soldedBySellers.clear();

	QSqlQuery query(QSqlDatabase::database(connectionName));
	query.prepare("SELECT Actions.title, Actions.performer, Places.title, Categories.name FROM Actions, Places, Categories WHERE Actions.id = :id AND Places.id = Actions.id_place AND Categories.id = Actions.id_category;");
	query.bindValue(":id", actionId);
	if(query.exec() && query.first())
	{
		mValid = true;
		title = query.value(0).toString();
		performer = query.value(1).toString();
		place = query.value(2).toString();
		category = query.value(3).toString();
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	query.prepare("SELECT price, id_market, id_user FROM Tickets WHERE id_action = :id");
	query.bindValue(":id", actionId);
	if(query.exec())
	{
		QHash<int, int> _soldedByMarkets;
		QHash<int, int> _soldedBySellers;

		while(query.next())
		{
			totalTicketsSolded++;
			totalSumm += query.value(0).toInt();

			int marketId = query.value(1).toInt();
			if(marketId == 0)
				soldedBySite++;
			else if(_soldedByMarkets.contains(marketId))
				_soldedByMarkets[marketId]++;
			else
				_soldedByMarkets.insert(marketId, 1);

			int sellerId = query.value(2).toInt();
			if(sellerId > 0)
			{
				if(_soldedBySellers.contains(sellerId))
					_soldedBySellers[sellerId]++;
				else
					_soldedBySellers.insert(sellerId, 1);
			}
		}

		QList<int> keys = _soldedByMarkets.keys();
		query.prepare("SELECT address FROM Markets WHERE id = :id;");
		for(int i = 0; i < keys.count(); i++)
		{
			query.bindValue(":id", keys[i]);
			if(query.exec() && query.first())
				soldedByMarkets.insert(query.value(0).toString(), _soldedByMarkets[keys[i]]);
		}

		keys = _soldedBySellers.keys();
		query.prepare("SELECT name FROM Users WHERE id = :id;");
		for(int i = 0; i < keys.count(); i++)
		{
			query.bindValue(":id", keys[i]);
			if(query.exec() && query.first())
				soldedBySellers.insert(query.value(0).toString(), _soldedBySellers[keys[i]]);
		}
	}
	else
		qDebug(qPrintable(query.lastError().text()));

	query.prepare("SELECT COUNT(id), SUM(penalty) FROM ReturnedTickets WHERE id_action = :id");
	query.bindValue(":id", actionId);
	if(query.exec() && query.first())
	{
		ticketsReturned = query.value(0).toInt();
		totalTicketsSolded += ticketsReturned;
		penaltySumm = query.value(1).toInt();
		totalSumm += penaltySumm;
	}
	else
		qDebug(qPrintable(query.lastError().text()));
	return mValid;
}

bool CComplitedAction::toQuery(QSqlQuery *query)
{
	bool ret = false;
	if(query && isValid())
	{
		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream << soldedByMarkets << soldedBySellers;

		query->prepare("INSERT INTO ComplitedActions VALUES(NULL, :title, :perforemer, :place, :category, :solded, :returned, :bySite, :sum, :penalty, :data);");
		query->bindValue(":title", title);
		query->bindValue(":perforemer", performer);
		query->bindValue(":place", place);
		query->bindValue(":category", category);
		query->bindValue(":solded", totalTicketsSolded);
		query->bindValue(":returned", ticketsReturned);
		query->bindValue(":bySite", soldedBySite);
		query->bindValue(":sum", totalSumm);
		query->bindValue(":penalty", penaltySumm);
		query->bindValue(":data", data);
		ret = true;
	}
	return ret;
}

void CComplitedAction::soldedFromByteArray(const QByteArray &data)
{
	QDataStream stream(data);
	stream >> soldedByMarkets >> soldedBySellers;
}
