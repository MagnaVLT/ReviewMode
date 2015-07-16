#include "../stdafx.h"
#include "DataHandler.h"

void DataHandler::init()
{
	this->connection = new DBWrapper();
	this->connection->conn("10.64.231.50", 3306, "fcm_gen3_hil_playback", "root", "Magnatkfkd");
	//this->connection->conn("10.64.231.50", 3306, "dev_hil_playback", "root", "Magnatkfkd");
	//this->connection->conn("192.168.0.9", 3306, "fcm_gen3_hil_playback", "root", "dkagh");
	//this->connection->conn("localhost", 3306, "fcm_gen3_hil_playback", "root", "hil");
}

DataHandler::DataHandler()
{
	this->init();
}

DataHandler::~DataHandler(void)
{

}