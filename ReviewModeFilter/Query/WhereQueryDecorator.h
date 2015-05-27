#pragma once
#include "QueryDecorator.h"
#include "Query.h"
#include <string>
using namespace std;

class WhereQueryDecorator: public QueryDecorator
{
public:
	WhereQueryDecorator(Query *qd);
	~WhereQueryDecorator(void);

	virtual string getQuery() = 0;
};