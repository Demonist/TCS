#include "cmarket.h"

static CMarket *market = 0;

CMarket::CMarket()
{
	mMarketId = 0;
	mSellerId = 0;
}

CMarket::~CMarket()
{
	market = 0;
}

CMarket* CMarket::instance()
{
	if(!market)
		market = new CMarket();
	return market;
}
