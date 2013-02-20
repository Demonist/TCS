#include "cticketidentifier.h"

CTicketIdentifier::CTicketIdentifier()
{
}

CTicketIdentifier::CTicketIdentifier(const QString &identifier)
{
	mIdentifier = identifier;
}

void CTicketIdentifier::render(QPainter *painter) const
{
	if(painter)
	{

	}
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
			case 13:
			{
				char key = mIdentifier.mid(9, 1).toInt() * 10 + mIdentifier.mid(2, 1).toInt();
				if(key)
				{
					QByteArray decrypted;
					QByteArray temp;

					temp = QByteArray::number(mIdentifier.mid(0, 2).toInt() ^ key);
					if(temp.size() < 2)
						temp.prepend('0');
					decrypted.append(temp);

					temp = QByteArray::number(mIdentifier.mid(3, 2).toInt() ^ key);
					if(temp.size() < 2)
						temp.prepend('0');
					decrypted.append(temp);

					temp = QByteArray::number(mIdentifier.mid(5, 2).toInt() ^ key);
					if(temp.size() < 2)
						temp.prepend('0');
					decrypted.append(temp);

					temp = QByteArray::number(mIdentifier.mid(7, 2).toInt() ^ key);
					if(temp.size() < 2)
						temp.prepend('0');
					decrypted.append(temp);

					temp = QByteArray::number(mIdentifier.mid(11, 2).toInt() ^ key);
					if(temp.size() < 2)
						temp.prepend('0');
					decrypted.append(temp);

					data = decrypted.mid(0, 4) + decrypted.mid(5);
				}
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
\param data содержит данные. Поддерживаются следующие форматы: размер 9 символов, только цифры.
*/
CTicketIdentifier CTicketIdentifier::generate(const QString &data)
{
	QString identifier;

	switch(data.size())
	{
		case 9:
		{
			bool conversion;
			identifier.toInt(&conversion, 10);
			if(conversion == true)
			{
				char key = 1 + qrand() % 100;
				char secondKey = qrand() % 10;

				QByteArray crypted;
				QByteArray temp;

				temp = QByteArray::number(data.mid(0, 2).toInt() ^ key);
				if(temp.size() < 2)
					temp.prepend('0');
				crypted.append(temp);

				temp = QByteArray::number(data.mid(2, 2).toInt() ^ key);
				if(temp.size() < 2)
					temp.prepend('0');
				crypted.append(temp);

				temp = QByteArray::number(((10 - secondKey) * 10 + data.mid(4, 1).toInt()) ^ key);
				if(temp.size() < 2)
					temp.prepend('0');
				crypted.append(temp);

				temp = QByteArray::number(data.mid(5, 2).toInt() ^ key);
				if(temp.size() < 2)
					temp.prepend('0');
				crypted.append(temp);

				crypted.append(QByteArray::number(secondKey));

				temp = QByteArray::number(data.mid(7, 2).toInt() ^ key);
				if(temp.size() < 2)
					temp.prepend('0');
				crypted.append(temp);

				crypted.insert(2, QByteArray::number(key % 10));
				crypted.insert(9, QByteArray::number(key / 10));

				identifier = crypted;
			}
		}
			break;
	}

	return CTicketIdentifier(identifier);
}

bool CTicketIdentifier::isValidIdentifier(const QString &identifier)
{
	switch(identifier.size())
	{
		case 13:
		{
			bool conversion;
			identifier.toInt(&conversion, 10);
			if(conversion == true)
			{
				char key = identifier.mid(9, 1).toInt() * 10 + identifier.mid(2, 1).toInt();
				if(key)
				{
					char duo = identifier.mid(4, 2).toInt() ^ key;
					return 10 - identifier.mid(8, 1).toInt() == duo / 10;
				}
			}
		}
			break;
	}

	return false;
}
