#include "../../stdafx.h"
#include "TemporalRetriever.h"
#include "../../Utils/Utilities.h"
#include <iostream>

using namespace std;

TemporalRetriever::TemporalRetriever(string tableName, vector<string> fields, vector<string> condition)
{	
	this->fields = fields;
	this->condition = condition;
	__super::tableName = tableName;
	this->setQuery();
	this->initDataContainer();
}

TemporalRetriever::TemporalRetriever(vector<string> fields, string query)
{
	this->fields = fields;
	__super::query = query;
	this->initDataContainer();
}

TemporalRetriever::TemporalRetriever()
{	
	
}

void TemporalRetriever::initDataContainer()
{
	for(unsigned int i = 0 ; i < this->fields.size() ; i++)
	{		
		this->result[this->fields.at(i)] = vector<string>();
	}
}

map<string, vector<string>> TemporalRetriever::getData(vector<string> fields, string query)
{
	this->fields = fields;
	__super::query = query;
	this->initDataContainer();
	this->result = __super::connection->getData(&this->fields, this->result, __super::query.c_str());
	return this->result;
}

map<string, vector<string>> TemporalRetriever::getData()
{
	this->result = __super::connection->getData(&this->fields, this->result, __super::query.c_str());
	return this->result;
}

void TemporalRetriever::clearData()
{
	this->result.clear();
}


void TemporalRetriever::setQuery()
{
	__super::query += "select ";
	for(unsigned int i = 0;i < this->fields.size()-1 ; i++){
		__super::query += this->fields.at(i) + ", ";
	}

	__super::query+=this->fields.at(this->fields.size()-1) + " ";
	__super::query+= " from " + __super::tableName;

	if(this->condition.size()>0){
		__super::query+= " where ";
		for(unsigned int i = 0;i < this->condition.size()-1 ; i++){
			__super::query += this->condition.at(i) + " and ";
		}
		__super::query+=this->condition.at(this->condition.size()-1);
	}
}

TemporalRetriever::~TemporalRetriever(void)
{

}