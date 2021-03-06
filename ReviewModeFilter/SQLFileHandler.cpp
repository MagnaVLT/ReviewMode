#include "stdafx.h"
#include "SQLFileHandler.h"
#include <stdio.h>
#include <string>
#include <map>
#include "DB/Retriever.h"
#include "DB/DataCenter/TemporalRetriever.h"
#include "XML/XMLHandler.h"
#include "DB/Updator.h"
#include "DB/DataCenter/TemporalUpdator.h"
#include "DB/TempDBHandle/TempDBMakeUpdator.h"
#include "Utils/Utilities.h"

#include <iostream>
#include <fstream>

using namespace std;

SQLFileHandler::SQLFileHandler(string TEMP_FILE, string id)
{
	this->id = id;
}

SQLFileHandler::~SQLFileHandler()
{
}

bool SQLFileHandler::check_integrity(string schemeXMLFile, string DB)
{
	return check_db(schemeXMLFile, DB);
}

void SQLFileHandler::init_temporal_database(string db, string id, string password, string sqlfile, bool flg_del_reviewed_events)
{
	string cmd_drop_db = "DROP database IF EXISTS  " + db + ";";
	string cmd_create_db = "create database " + db + ";";
	(new TempDBMakeUpdator(cmd_drop_db))->execute();
	(new TempDBMakeUpdator(cmd_create_db))->execute();

	string command = "mysql -u" + id + " -p" + password + " " + db + " < " + "\"" + sqlfile + "\"";
	int result = MagnaUtil::start_process_with_waiting(command);
}

void SQLFileHandler::delete_temporal_database(string db, string id, string password)
{
	string cmd_drop_db = "DROP database IF EXISTS  " + db + ";";
	(new TempDBMakeUpdator(cmd_drop_db))->execute();
}


bool SQLFileHandler::check_db(string schemeXMLFile, string DB)
{
	if(!MagnaUtil::isExist(schemeXMLFile))
	{
		MagnaUtil::show_message("The description of DB schema is required!");
		return false;
	}

	vector<string> table_list = getTablesFromXML(schemeXMLFile);
	if(!table_list.empty())
	{
		vector<string> tableListFromDB = this->getTableListFrom(DB);
		for each(string table in table_list)
		{
			if(check_table_existence(table, tableListFromDB)==false) 
			{
				MagnaUtil::show_message(table + " is not exist.");
				return false;
			}
	
			vector<string> fieldsFromXML = getFieldsFromXML(schemeXMLFile, table);
			vector<string> fieldsFromDB = getFieldsFromDB(table, DB);

			//redandant start-------------------------------------------------
			vector<int> deleteItem;
			int size = fieldsFromXML.size();
			for(int i = size-1 ; i >=0; i--){
				if(fieldsFromXML.at(i)=="updator"||fieldsFromXML.at(i)=="insertor"||fieldsFromXML.at(i)=="updatedtime"||fieldsFromXML.at(i)=="insertedtime")
					deleteItem.push_back(i);
			}
			for each(int i in deleteItem) fieldsFromXML.erase(fieldsFromXML.begin() + i);
			//
			deleteItem.clear();
			size = fieldsFromDB.size();
			for(int i = size-1 ; i >=0; i--){
				if(fieldsFromDB.at(i)=="updator"||fieldsFromDB.at(i)=="insertor"||fieldsFromDB.at(i)=="updatedtime"||fieldsFromDB.at(i)=="insertedtime")
					deleteItem.push_back(i);
			}
			for each(int i in deleteItem) fieldsFromDB.erase(fieldsFromDB.begin() + i);
			if(deleteItem.size()==0)
			{
				string query = "ALTER TABLE event_report   ";
				query += "ADD COLUMN updator VARCHAR(6) NOT NULL DEFAULT '0' AFTER PredefinedAnnotationID, ";
				query += "ADD COLUMN insertor VARCHAR(6) NOT NULL DEFAULT '0' AFTER updator, ";
				query += "ADD COLUMN updatedtime BIGINT(20) NOT NULL DEFAULT 0 AFTER insertor, ";
				query += "ADD COLUMN insertedtime BIGINT(20) NOT NULL DEFAULT 0 AFTER updatedtime;";
				(new TemporalUpdator(query))->execute();
			}
			//redandant end----------------------------------------------------

			string curTime = MagnaUtil::bigIntegerToString(MagnaUtil::getCurrentSystemTime());
			string query = "update event_report set insertor = '" + this->id +"', insertedtime = " + curTime + ";";
			(new TemporalUpdator(query))->execute();

			if(MagnaUtil::compare_string_vector(fieldsFromXML, fieldsFromDB)==false)
			{
				MagnaUtil::show_message("Fields in "+ table +" is not correct.");
				return false;
			}
		}
	}
	
	setUploadedEvents();
	return true;
}

void SQLFileHandler::setUploadedEvents()
{
	vector<string> field;
	field.push_back("reportid");
	field.push_back("vin");
	string query = "select reportid, vin from event_report;";

	uploaded_events = (new TemporalRetriever())->getData(field, query);
}

vector<string> SQLFileHandler::getFieldsFromXML(string schemeXMLFile, string table)
{
	XMLHandler xmlHandle(schemeXMLFile);
	return xmlHandle.getNodeListAtSecondLevel(table);
}

vector<string> SQLFileHandler::getTablesFromXML(string schemeXMLFile)
{
	XMLHandler xmlHandle(schemeXMLFile);
	return xmlHandle.getNodeNameListAtFirstLevel("backup");
}

vector<string> SQLFileHandler::getFieldsFromDB(string table, string DB)
{
	vector<string> field;
	field.push_back("field");
	string query = "describe " + table + ";";
	map<string, vector<string>> containers = (new TemporalRetriever(field, query))->getData();
	return containers["field"];
}

vector<string> SQLFileHandler::getTableListFrom(string DB)
{
	vector<string> field;
	string f = "Tables_in_" + DB;
	field.push_back(f);
	vector<string> result;
	string query = "show tables;";
	map<string, vector<string>> containers = (new TemporalRetriever(field, query))->getData();
	result = containers[f.c_str()];
	return result;
}

bool SQLFileHandler::check_table_existence(string table, vector<string> table_list_in_db)
{
	return MagnaUtil::Contains(table_list_in_db, table);
}


bool SQLFileHandler::check_database_existence(string DB)
{
	vector<string> field;
	string f = "Database";
	field.push_back(f);
	string query = "show databases;";
	map<string, vector<string>> containers = (new TemporalRetriever(field, query))->getData();

	return MagnaUtil::Contains(containers[f.c_str()], DB);
}


void SQLFileHandler::insertNewRecords(QWidget* m_pFilterWidget, string dbName, string tmpFileName)
{

	QProgressDialog *progress = getProgressDialog(m_pFilterWidget);
	progress->show();
	MagnaUtil::delete_file(tmpFileName);

	string query = "SELECT * from "+dbName+" INTO OUTFILE '"+tmpFileName+"';";
	(new TemporalUpdator(query))->execute();
	string lock_query = "LOCK TABLES `"+dbName+"` WRITE;";
	(new Updator(dbName))->execute();
	string load_query = "LOAD DATA LOCAL INFILE '"+tmpFileName+"' INTO TABLE " + dbName +";";
	(new Updator(load_query))->execute();
	(new Updator("UNLOCK TABLES;"))->execute();
	MagnaUtil::delete_file(tmpFileName);

	progress->hide();
}

QProgressDialog* SQLFileHandler::getProgressDialog(QWidget* m_pFilterWidget)
{
	QProgressDialog *progress = new QProgressDialog(m_pFilterWidget);
	progress->setWindowTitle(QString("Copying SQL File to Database..."));
	progress->autoClose();
	progress->setLabelText(QString("Inserting Events..."));
	progress->setFixedWidth(500);
	progress->setWindowModality(Qt::WindowModal);
	progress->setRange(0,100);

	return progress;
}
