#pragma once
#include <string>
#include "Query.h"
using namespace std;

class QueryDecorator: public Query
{
public:
	QueryDecorator(){}
	QueryDecorator(Query* qd){}
	~QueryDecorator(void){}

	virtual string getQuery()=0;
};