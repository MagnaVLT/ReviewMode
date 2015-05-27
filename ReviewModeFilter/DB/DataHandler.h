#pragma once
#include <string>
#include <iostream>
#include "DBWrapper.h"

using namespace std;

class DataHandler
{
public:
	DataHandler();
	~DataHandler(void);
	
protected:
	string tableName;
	string query;
	void init();
	DBWrapper* connection;
};