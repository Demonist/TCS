#ifndef CIMAGES_H
#define CIMAGES_H

#include <QtCore>
#include <QtSql>

/**
Класс для работы с изображениями в базе данных.
Данный класс выполнен в виде синглетона.

Позволяет добавлять, изменить и удалять изображения из базы данных.
Все изображения кешируются.

@author Demonist
*/
class CImages
{
private:
	QString mConnectionName;	//! Имя подключения к базе данных.
	QMutex mInsertMutex;

private:
	explicit CImages();	//private constructor for singleton.

public:
	static CImages* instance();
	static CImages* instance(const QString &connectionName);
	~CImages();

	inline void setConnectionName(const QString &connectionName);
	inline QString connectionName() const;

	QPixmap image(const int id);

	int insert(const QPixmap &image);
	inline int insert(const QString &imageFileName);

	void update(const int id, const QPixmap &image);
	inline void update(const int id, const QString &imageFileName);

	void remove(const int id);

	static QString imageFileName(const int id);
	static QPixmap image(const int id, const QString &connectionName);
};

//inline implementation:

inline void CImages::setConnectionName(const QString &connectionName)
{
	mConnectionName = connectionName;
}
inline QString CImages::connectionName() const
{
	return mConnectionName;
}

inline int CImages::insert(const QString &imageFileName)
{
	return insert(QPixmap(imageFileName));
}

inline void CImages::update(const int id, const QString &imageFileName)
{
	update(id, QPixmap(imageFileName));
}

#endif // CIMAGES_H
