#pragma once
#include <vector>
#include <string>
#include <map>
#include "DataHandler.h"

using namespace std;

class Retriever: public DataHandler
{
public:
	
	Retriever(string tableName, vector<string> fields, vector<string> condition);
	Retriever(vector<string> fields, string query);
	Retriever();

	~Retriever(void);
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

