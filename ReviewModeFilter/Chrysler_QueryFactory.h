#pragma once
#include "Query/Query.h"

class Chrysler_QueryFactory: public Query
{
public:
	Chrysler_QueryFactory(void);
	~Chrysler_QueryFactory(void);
	virtual string getQuery();
};