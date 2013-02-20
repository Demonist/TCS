#ifndef CTICKETIDENTIFIER_H
#define CTICKETIDENTIFIER_H

#include <QtCore>
#include <QPainter>

class CTicketIdentifier
{
private:
	QString mIdentifier;

public:
	explicit CTicketIdentifier();
	explicit CTicketIdentifier(const QString &identifier);

	void render(QPainter *painter) const;
	QString data() const;

	static CTicketIdentifier generate(const QString &data);
	static bool isValidIdentifier(const QString &identifier);

	inline QString identifier() const;
	inline void setIdentifier(const QString &identifier);

	inline bool isValid() const;
};

//inline implementation:

inline QString CTicketIdentifier::identifier() const
{
	return mIdentifier;
}
inline void CTicketIdentifier::setIdentifier(const QString &identifier)
{
	mIdentifier = identifier;
}

inline bool CTicketIdentifier::isValid() const
{
	return isValidIdentifier(mIdentifier);
}

#endif // CTICKETIDENTIFIER_H
