#include "../stdafx.h"
#include "Retriever.h"
#include "../Utils/Utilities.h"
#include <iostream>

using namespace std;

Retriever::Retriever(string tableName, vector<string> fields, vector<string> condition)
{	
	this->fields = fields;
	this->condition = condition;
	__super::tableName = tableName;
	this->setQuery();
	this->initDataContainer();
}

Retriever::Retriever(vector<string> fields, string query)
{
	this->fields = fields;
	__super::query = query;
	this->initDataContainer();
}

Retriever::Retriever()
{

}

void Retriever::initDataContainer()
{
	for(unsigned int i = 0 ; i < this->fields.size() ; i++)
	{		
		this->result[this->fields.at(i)] = vector<string>();
	}
}

map<string, vector<string>> Retriever::getData(vector<string> fields, string query)
{
	this->fields = fields;
	__super::query = query;
	this->initDataContainer();
	this->result = __super::connection->getData(&this->fields, this->result, __super::query.c_str());
	return this->result;
}

map<string, vector<string>> Retriever::getData()
{
	this->result = __super::connection->getData(&this->fields, this->result, __super::query.c_str());
	return this->result;
}

void Retriever::clearData()
{
	this->result.clear();
}


void Retriever::setQuery()
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

Retriever::~Retriever(void)
{

}