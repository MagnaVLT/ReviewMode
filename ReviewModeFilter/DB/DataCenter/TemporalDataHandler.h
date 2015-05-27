#pragma once
#include <string>
#include <iostream>
#include "TemporalDBWrapper.h"

using namespace std;

class TemporalDataHandler
{
public:
	TemporalDataHandler();
	~TemporalDataHandler(void);
	
protected:
	string tableName;
	string query;
	void init();
	TemporalDBWrapper* connection;
};