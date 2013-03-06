#ifndef CMARKET_H
#define CMARKET_H

#include <QtCore>

class CMarket
{
protected:
	int mMarketId;
	int mSellerId;
	QString mSeller;

private:
	CMarket();

public:
	~CMarket();

	static CMarket* instance();

	inline int marketId() const;
	inline int sellerId() const;
	inline QString seller() const;

	inline void setMarketId(const int marketId);
	inline void setSeller(const int sellerId, const QString &seller);
};

//inline implementation:

inline int CMarket::marketId() const
{
	return mMarketId;
}
inline void CMarket::setMarketId(const int marketId)
{
	mMarketId = marketId;
}

inline int CMarket::sellerId() const
{
	return mSellerId;
}
inline QString CMarket::seller() const
{
	return mSeller;
}
inline void CMarket::setSeller(const int sellerId, const QString &seller)
{
	mSellerId = sellerId;
	mSeller = seller;
}

#endif // CMARKET_H
