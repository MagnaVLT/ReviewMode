#include "../../stdafx.h"
#include "TempDBMakeUpdator.h"
#include <iostream>

using namespace std;


TempDBMakeUpdator::TempDBMakeUpdator()
{

}

TempDBMakeUpdator::TempDBMakeUpdator(string query)
{
	__super::query = query;
}

void TempDBMakeUpdator::execute()
{
	__super::connection->update((__super::query).c_str());
}

void TempDBMakeUpdator::execute(string query)
{
	__super::connection->update(query.c_str());
}