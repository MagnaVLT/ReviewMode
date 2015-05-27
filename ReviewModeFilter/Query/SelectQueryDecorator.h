#pragma once
#include "QueryDecorator.h"
#include "Query.h"
#include <string>
using namespace std;

class SelectQueryDecorator : public QueryDecorator
{
public:
	SelectQueryDecorator(void);
	~SelectQueryDecorator(void);
	virtual string getQuery() = 0;
};
