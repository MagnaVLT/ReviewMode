#pragma once
#include <string>
#include <iostream>
#include "TempDBMakeDBWrapper.h"

using namespace std;

class TempDBMakeDataHandler
{
public:
	TempDBMakeDataHandler();
	~TempDBMakeDataHandler(void);
	
protected:
	string tableName;
	string query;
	void init();
	TempDBMakeDBWrapper* connection;
};