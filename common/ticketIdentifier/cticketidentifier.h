#ifndef CTICKETIDENTIFIER_H
#define CTICKETIDENTIFIER_H

#include <QtCore>
#include <QPainter>
#include "ean13.h"

class CTicketIdentifier
{
private:
	QString mBarCode;

public:
	 explicit CTicketIdentifier();
	 explicit CTicketIdentifier(const QString &barCode);

	void render(const int w, const int h, QPainter *painter) const;
	inline void render(const QSize &imageSize, QPainter *painter) const;

	QString identifier() const;

	static CTicketIdentifier generate(const QString &identifier);
	static CTicketIdentifier generate();

	static bool isValidBarCode(const QString &barCode);

	inline QString barCode() const;
	inline void setBarCode(const QString &barCode);

	inline bool isValid() const;
};

//inline implementation:

inline QString CTicketIdentifier::barCode() const
{
	return mBarCode;
}
inline void CTicketIdentifier::setBarCode(const QString &barCode)
{
	mBarCode = barCode;
}

inline bool CTicketIdentifier::isValid() const
{
	return isValidBarCode(mBarCode);
}

inline void CTicketIdentifier::render(const QSize &imageSize, QPainter *painter) const
{
	return render(imageSize.width(), imageSize.height(), painter);
}

#endif // CTICKETIDENTIFIER_H
