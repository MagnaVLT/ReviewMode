#pragma once
#include <my_global.h>
#include <iostream>
#include <mysql.h>
#include <vector>
#include <string>
#include <map>

using namespace std;


class DBWrapper
{
public:
	DBWrapper(void){}
	~DBWrapper(void);
	static MYSQL* conn(const char*, int, const char*, const char*, const char*);
	map<string, vector<string>> getData(vector<string> *field, map<string, vector<string>> container, const char* query);
	void update(const char* query);
	static bool instanceFlag;

private:
	static MYSQL mysql;
};

