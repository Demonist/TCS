#include "cimages.h"

#define LOCAL "imagesCache"

static CImages *images = 0;

QString databaseFilename()
{
	QDir path(QCoreApplication::arguments().value(0));
	path.cdUp();
	path.mkdir("imagesCache");
	path.cd("imagesCache");
	return path.absoluteFilePath("images.cache");
}

//private:

CImages::CImages()
{
	mConnectionName = "common";

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", LOCAL);
	db.setDatabaseName(databaseFilename());
	if(db.open())
	{
		QSqlQuery query(db);
		if(!query.exec("CREATE TABLE IF NOT EXISTS ImagesCache(id INTEGER PRIMARY KEY, dateTime DATETIME NULL);"))
			qDebug(qPrintable(query.lastError().text()));
	}
}

CImages::CImages(const CImages &obj)
{

}

CImages& CImages::operator =(const CImages &obj)
{
	return *this;
}

//public:

/**
Возвращает экземпляр класса, который создается при необходимости.
*/
CImages* CImages::instance()
{
	if(images == 0)
		images = new CImages();

	return images;
}

/**
Возвращает экземпляр класса, который создается при необходимости.
Меняет имя подключения к базе данных на \a connectionName.
*/
CImages* CImages::instance(const QString &connectionName)
{
	if(images == 0)
		images = new CImages();
	images->mConnectionName = connectionName;
	return images;
}

CImages::~CImages()
{
	{
		QSqlDatabase db = QSqlDatabase::database(LOCAL);
		db.close();
	}
	QSqlDatabase::removeDatabase(LOCAL);
	images = 0;
}

/**
Возвращает изображение с указанным \a id.
Возвращается изображение из кеша, которое, если это необходимо, загружается или обновляется из базы данных с именем подключения указанным в \a mConnectionName.
*/
QPixmap CImages::image(const int id)
{
	QPixmap ret;

	QSqlQuery localQuery(QSqlDatabase::database(LOCAL));
	localQuery.prepare("SELECT dateTime FROM ImagesCache WHERE id = :id;");
	localQuery.bindValue(":id", id);

	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("SELECT dateTime FROM Images WHERE id = :id;");
	query.bindValue(":id", id);

	if(query.exec() && localQuery.exec()
	   && query.first())
	{
		bool needLoad = true;
		enum UpdateType{Insert, Update} updateType;

		if(localQuery.first())
		{
			if(query.value(0) == localQuery.value(0)
			   && QFile::exists(imageFileName(id)))
				needLoad = false;
			else
				updateType = Update;
		}
		else
			updateType = Insert;

		if(needLoad)
		{
			query.prepare("SELECT image, dateTime FROM Images WHERE id = :id;");
			query.bindValue(":id", id);
			if(query.exec() && query.first())
			{
				if(ret.loadFromData(query.value(0).toByteArray(), "PNG")
				   && ret.save(imageFileName(id), "PNG", 100))
				{
					if(updateType == Insert)
						localQuery.prepare("INSERT INTO ImagesCache VALUES(:id, :date);");
					else
						localQuery.prepare("UPDATE ImagesCache SET dateTime = :date WHERE id = :id;");
					localQuery.bindValue(":id", id);
					localQuery.bindValue(":date", query.value(1));

					if(!localQuery.exec())
						qDebug(qPrintable(localQuery.lastError().text()));
				}
				else
					qDebug("CImages::image [static]: Wrong save logic!");	////
			}
		}
		else
		{
			if(!ret.load(imageFileName(id)))
				qDebug("CImages::image [static]: Wrong load logic!");	////
		}
	}
	return ret;
}

/**
Добавляет изображение в базу данных с именем подкючения, указанным в \a mConnectionName.
При успешном добавлении, вносит соответствующие изменения в кеш.

\return Возвращет идентификатор изображения в базе данных или 0 в случае ошибки.
\warning Несмотря на потоко-безопасность, данную функцию категорически запрещается использовать одновременно на нескольких машинах.
*/
int CImages::insert(const QPixmap &image)
{
	QMutexLocker locker(&mInsertMutex);

	int ret = 0;
	if(image.isNull() == false)
	{
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		if(buffer.open(QBuffer::WriteOnly)
		   && image.save(&buffer, "PNG"))
		{
			buffer.close();

			QSqlDatabase db = QSqlDatabase::database(mConnectionName);
			QSqlQuery query(db);

			QString curDate;
			if(db.driverName() == "QSQLITE")
				curDate = "datetime('now')";
			else
				curDate = "NOW()";

			query.exec("BEGIN TRANSACTION;");

			query.prepare("INSERT INTO Images VALUES(NULL, :image, " + curDate + ");");
			query.bindValue(":image", byteArray);
			if(query.exec()
			   && query.exec("SELECT MAX(id) FROM Images;")
			   && query.first())
			{
				ret = query.value(0).toInt();

				query.prepare("SELECT dateTime FROM Images WHERE id = :id;");
				query.bindValue(":id", ret);
				if(query.exec() && query.first())
				{
					QFile::remove(imageFileName(ret));
					if(image.save(imageFileName(ret), "PNG"))
					{
						QSqlQuery localQuery(QSqlDatabase::database(LOCAL));
						localQuery.prepare("INSERT INTO ImagesCache VALUES(:id, :date);");
						localQuery.bindValue(":id", ret);
						localQuery.bindValue(":date", query.value(0));
						localQuery.exec();
					}
				}
			}
			query.exec("COMMIT TRANSACTION;");
		}
	}
	return ret;
}

/**
Обновляет изображение в базе данных с именем подкючения, указанным в \a mConnectionName.
Так же обновляется кешированный файл и информация в кеше.
*/
void CImages::update(const int id, const QPixmap &image)
{
	QByteArray byteArray;
	QBuffer buffer(&byteArray);
	if(buffer.open(QBuffer::WriteOnly)
	   && image.save(&buffer, "PNG"))
	{
		buffer.close();

		QSqlDatabase db = QSqlDatabase::database(mConnectionName);
		QSqlQuery query(db);

		QString curDate;
		if(db.driverName() == "QSQLITE")
			curDate = "datetime('now')";
		else
			curDate = "NOW()";

		query.prepare("UPDATE Images SET image = :image, dateTime = " + curDate + " WHERE id = :id;");
		query.bindValue(":image", byteArray);
		query.bindValue(":id", id);

		if(query.exec())
		{
			QFile::remove(imageFileName(id));
			query.prepare("SELECT dateTime FROM Images WHERE id = :id;");
			query.bindValue(":id", id);
			if(query.exec() && query.first()
			   && image.save(imageFileName(id), "PNG"))
			{
				QSqlQuery localQuery(QSqlDatabase::database(LOCAL));
				localQuery.prepare("UPDATE ImagsCache SET dateTime = :date WHERE id = :id;");
				localQuery.bindValue(":date", query.value(0));
				localQuery.bindValue(":id", id);
				localQuery.exec();
			}
		}
	}
}

/**
Удаляет изображение из базы данных с именем подкючения, указанным в \a mConnectionName.
Так же удаляется кешированный файл и информация из кеша.
*/
void CImages::remove(const int id)
{
	QSqlQuery query(QSqlDatabase::database(mConnectionName));
	query.prepare("DELETE FROM Images WHERE id = :id;");
	query.bindValue(":id", id);
	query.exec();

	QSqlQuery localQuery(QSqlDatabase::database(LOCAL));
	localQuery.prepare("DELETE FROM ImagesCache WHERE id = :id;");
	localQuery.bindValue(":id", id);
	localQuery.exec();
	QFile::remove(imageFileName(id));
}

//static:

/**
Возвращает имя файла для изображения с указанным \a id.
Проверка на существование файла не производится.
*/
QString CImages::imageFileName(const int id)
{
	QDir path(QCoreApplication::arguments().value(0));
	path.cdUp();
	path.mkdir("imagesCache");
	path.cd("imagesCache");
	return path.absoluteFilePath(QString("%1.png").arg(id));
}

/**
Возвращает изображение с указанным \a id.
Возвращается изображение из кеша, которое, если это необходимо, загружается или обновляется из базы данных с именем подключения \a connectionName.

\note В данной функции создается временное подключение к базе данных кеша изображений, которое затем удаляется.
Так как это может сказаться на производительности - рекомендуется использовать \b не статичную функцию:
\codeline CImages::instance().image();
*/
QPixmap CImages::image(const int id, const QString &connectionName)
{
	QPixmap ret;

	const char* const tempConnectionName = "tempImagesCache";
	{
		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", tempConnectionName);
		db.setDatabaseName(databaseFilename());
		if(db.open())
		{
			QSqlQuery localQuery(db);
			if(!localQuery.exec("CREATE TABLE IF NOT EXISTS ImagesCache(id INTEGER PRIMARY KEY, dateTime DATETIME NULL);"))
				qDebug(qPrintable(localQuery.lastError().text()));

			localQuery.prepare("SELECT dateTime FROM Images WHERE id = :id;");
			localQuery.bindValue(":id", id);

			QSqlQuery query(QSqlDatabase::database(connectionName));
			query.prepare("SELECT dateTime FROM Images WHERE id = :id;");
			query.bindValue(":id", id);

			if(query.exec() && localQuery.exec()
			   && query.first())
			{
				bool needLoad = true;
				enum UpdateType{Insert, Update} updateType;

				if(localQuery.first())
				{
					if(query.value(0) == localQuery.value(0)
					   && QFile::exists(imageFileName(id)))
						needLoad = false;
					else
						updateType = Update;
				}
				else
					updateType = Insert;

				if(needLoad)
				{
					query.prepare("SELECT image, dateTime FROM Images WHERE id = :id;");
					query.bindValue(":id", id);
					if(query.exec() && query.first())
					{
						if(ret.loadFromData(query.value(0).toByteArray(), "PNG")
						   && ret.save(imageFileName(id), "PNG", 100))
						{
							if(updateType == Insert)
								localQuery.prepare("INSERT INTO ImagesCache VALUES(:id, :date);");
							else
								localQuery.prepare("UPDATE ImagesCache SET dateTime = :date WHERE id = :id;");
							localQuery.bindValue(":id", id);
							localQuery.bindValue(":date", query.value(1));

							if(!localQuery.exec())
								qDebug(qPrintable(localQuery.lastError().text()));
						}
						else
							qDebug("CImages::image [static]: Wrong save logic!");	////
					}
				}
				else
				{
					if(!ret.load(imageFileName(id)))
						qDebug("CImages::image [static]: Wrong load logic!");	////
				}
			}

			db.close();
		}
	}
	QSqlDatabase::removeDatabase(tempConnectionName);

	return ret;
}
