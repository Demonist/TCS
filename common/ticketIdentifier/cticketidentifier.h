#ifndef CTICKETIDENTIFIER_H
#define CTICKETIDENTIFIER_H

#include <QtCore>
#include <QPainter>
#include "ean13.h"

class CTicketIdentifier
{
private:
	QString mIdentifier;

public:
	 explicit CTicketIdentifier();
	 explicit CTicketIdentifier(const QString &identifier);

	void render(const int w, const int h, QPainter *painter) const;
	inline void render(const QSize &imageSize, QPainter *painter) const;

	QString data() const;

	static CTicketIdentifier generate(const QString &data);
	static CTicketIdentifier generate();

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

inline void CTicketIdentifier::render(const QSize &imageSize, QPainter *painter) const
{
	return render(imageSize.width(), imageSize.height(), painter);
}

#endif // CTICKETIDENTIFIER_H
