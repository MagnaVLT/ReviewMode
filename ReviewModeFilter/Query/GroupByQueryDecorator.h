#pragma once
#include "QueryDecorator.h"
#include "Query.h"
#include <string>
using namespace std;

class GroupByQueryDecorator: public QueryDecorator
{
public:
	GroupByQueryDecorator(Query *qd);
	~GroupByQueryDecorator(void);

	virtual string getQuery() = 0;
};
