#include "../../stdafx.h"
#include "TemporalUpdator.h"
#include <iostream>

using namespace std;


TemporalUpdator::TemporalUpdator()
{

}

TemporalUpdator::TemporalUpdator(string query)
{
	__super::query = query;
}

void TemporalUpdator::execute()
{
	__super::connection->update((__super::query).c_str());
}

void TemporalUpdator::execute(string query)
{
	__super::connection->update(query.c_str());
}