#ifndef CCACHECHECKER_H
#define CCACHECHECKER_H

#include <QtSql>

/**
  Класс для проверки кеширования.
Позволяет проверить не обновились ли закешированные данные.
Данные могут кешироваться где угодно, например в интерфейсе приложения.

\warning Данный класс \b НЕ производит кеширование, а только проверяет нужно ли обновление. Кеширование реализуется программистом.

Пример использования:
\code
	void updateMyData()
	{
		static CCacheChecker cacheChecker("table1 table2, table3", "myConnectionName");
		if(cacheChecker.isNeedUpdate())
		{
			// ... Производится обновление кешированных данных.

			if(dataWasUpdated())	//Проверка на успешность обновления.
				cacheChecker.setUpdated();	//Помечаем данные как обновленные. Информация об обновлении берется из последнего вызова функции isNeedUpdate.
		}
	}
\endcode

\author Demonist
*/
class CCacheChecker
{
public:
	explicit CCacheChecker(const QString &tables, const QString &connectionName);
	explicit CCacheChecker(const QStringList &tables, const QString &connectionName);

	bool isNeedUpdate() const;
	inline void setUpdated();

protected:
	void initialize(const QStringList &tables, const QString &connectionName);

protected:
	QString mConnectionName;
	QList<QString> mTables;
	QMap<QString, QDateTime> mLastUpdate;
	mutable QMap<QString, QDateTime> mLastCheck;
};

//inline implementation:

/**
  Функция помечает, что кешируемые данные обновлены.
Перед вызовом этой функции \b обязательно должна быть произведена проверка необходимости обновления с помощью \fn isNeedUpdate().
*/
inline void CCacheChecker::setUpdated()
{
	mLastUpdate = mLastCheck;
}

#endif // CCACHECHECKER_H
