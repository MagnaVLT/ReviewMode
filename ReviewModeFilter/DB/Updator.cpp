#include "../stdafx.h"
#include "Updator.h"
#include <iostream>

using namespace std;

Updator::Updator()
{

}

Updator::Updator(string query)
{
	__super::query = query;
}

void Updator::execute()
{
	__super::connection->update((__super::query).c_str());
}

void Updator::execute(string query)
{
	__super::connection->update(query.c_str());
}