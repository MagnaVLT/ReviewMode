#pragma once
#include <vector>
#include <string>
#include <map>
#include "TemporalDataHandler.h"

using namespace std;

class TemporalRetriever: public TemporalDataHandler
{
public:
	
	TemporalRetriever(string tableName, vector<string> fields, vector<string> condition);
	TemporalRetriever(vector<string> fields, string query);
	TemporalRetriever();

	~TemporalRetriever(void);
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

