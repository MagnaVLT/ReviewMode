#include "../../stdafx.h"
#include "TempDBMakeDataHandler.h"

void TempDBMakeDataHandler::init()
{
	this->connection = new TempDBMakeDBWrapper();
	this->connection->conn("localhost", 3306, "mysql", "root", "hil");
}

TempDBMakeDataHandler::TempDBMakeDataHandler()
{
	this->init();
}

TempDBMakeDataHandler::~TempDBMakeDataHandler(void)
{

}