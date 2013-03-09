#ifndef CCOMPLITEDACTION_H
#define CCOMPLITEDACTION_H

#include <QtCore>
#include <QtSql>

class CComplitedAction
{
protected:
	bool mValid;

public:
	int totalTicketsSolded;	//! Общее кол-во проданных билетов, вместе с возвращенными.
	int ticketsReturned;	//! Кол-во возвращенных билетов.
	int totalSumm;			//! Общая стоимость проданных билетов, вместе с неустойками.
	int penaltySumm;		//! Прибыль с неустоек при возврате билета.
	int soldedBySite;		//! Кол-во проданых билетов через сайт.
	QString title;
	QString performer;
	QString place;
	QString category;
	QHash<QString, int> soldedByMarkets;
	QHash<QString, int> soldedBySellers;

public:
	CComplitedAction();

	bool makeFromAction(const int actionId, const QString &connectionName);
	bool toQuery(QSqlQuery *query);
	void soldedFromByteArray(const QByteArray &data);

	inline bool isValid() const;
};

//inline implementation:

inline bool CComplitedAction::isValid() const
{
	return mValid;
}

#endif // CCOMPLITEDACTION_H
