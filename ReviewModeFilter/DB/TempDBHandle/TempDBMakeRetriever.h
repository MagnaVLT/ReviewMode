#pragma once
#include <vector>
#include <string>
#include <map>
#include "TempDBMakeDataHandler.h"

using namespace std;

class TempDBMakeRetriever: public TempDBMakeDataHandler
{
public:
	
	TempDBMakeRetriever(string tableName, vector<string> fields, vector<string> condition);
	TempDBMakeRetriever(vector<string> fields, string query);
	TempDBMakeRetriever();

	~TempDBMakeRetriever(void);
	map<string, vector<string>> getData();
	map<string, vector<string>> getData(vector<string> fields, string query);
	
	void clearData();

private:
	
	void setQuery();
	void initDataContainer();

	vector<string> fields;
	vector<string> condition;
	map<string, vector<string>> result;
};

