#pragma once
#include "QueryDecorator.h"
#include "Query.h"
#include <string>
using namespace std;

class FromQueryDecorator: public QueryDecorator
{
public:
	FromQueryDecorator(Query *qd);
	~FromQueryDecorator(void);

	virtual string getQuery() = 0;
};