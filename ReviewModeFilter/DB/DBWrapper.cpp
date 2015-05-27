#include "../stdafx.h"
#include "DBWrapper.h"
#include <string>
#include <QtGui/QMessageBox>

bool DBWrapper::instanceFlag = false;
MYSQL DBWrapper::mysql;


MYSQL* DBWrapper::conn(const char* host, int port, const char* db, const char* id, const char* pw)
{

	if(!instanceFlag){
		mysql_init(&mysql);
		mysql_real_connect(&mysql, host, id, pw,db, 3306, NULL, 0);
		instanceFlag=true;
	}
	
	return &mysql;
}

map<string, vector<string>> DBWrapper::getData(vector<string> *field, map<string, vector<string>> container, const char* query){
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_query(&mysql, query);
	res = mysql_store_result(&mysql);
	int count = mysql_num_fields(res);

	while(row=mysql_fetch_row(res)){
		
		for(unsigned int i = 0 ; i < field->size() ; i++)
		{
			if(row[i]==NULL)
			{
				container[field->at(i)].push_back("");
			}else{
				container[field->at(i)].push_back(row[i]);
			}
			
		}
	}

	mysql_free_result(res);
	return container;
}

void DBWrapper::update(const char* query)
{
	mysql_query(&mysql, query);
}



DBWrapper::~DBWrapper(void)
{
	instanceFlag = false;
	mysql_close(&this->mysql);
}