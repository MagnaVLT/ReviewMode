#include "../../stdafx.h"
#include "TemporalDataHandler.h"

void TemporalDataHandler::init()
{
	this->connection = new TemporalDBWrapper();
	this->connection->conn("localhost", 3306, "temporal_database", "root", "hil");
}

TemporalDataHandler::TemporalDataHandler()
{
	this->init();
}

TemporalDataHandler::~TemporalDataHandler(void)
{

}