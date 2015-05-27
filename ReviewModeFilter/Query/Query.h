#pragma once
#include <string>
using namespace std;


class Query
{
public:
	Query(void){}
	~Query(void){}

	virtual string getQuery() = 0;
};