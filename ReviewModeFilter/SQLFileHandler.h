#pragma once
#include <string>
#include <vector>
#include "DB/DataCenter/TemporalRetriever.h"
#include <QtGui/QApplication>
#include <QtGui/QProgressDialog>
#include <QtGui/QWidget>

using namespace std;
class SQLFileHandler
{
public:
	SQLFileHandler(string TEMP_FILE);
	~SQLFileHandler(void);

	bool check_integrity(string schemeFile, string TEMP_DB);
	void init_temporal_database(string db, string id, string password, string sqlfile, bool flg_del_reviewed_events);
	bool check_db(string schemeFile, string DB);
	vector<string> getFieldsFromXML(string schemeFile, string table);
	vector<string> getFieldsFromDB(string table, string DB);
	vector<string> getTablesFromXML(string schemeFile);
	void insertNewRecords(QWidget* m_pFilterWidget, string dbName, string tmpFileName);
	void delete_temporal_database(string db, string id, string password);
private:
	void addExistingReportID(string temp_stored_file, vector<string> *reportid_list);
	bool check_table_existence(string table, vector<string> table_list_in_db);
	bool check_database_existence(string DB);
	vector<string> getTableListFrom(string DB);
	QProgressDialog* getProgressDialog(QWidget* m_pFilterWidget);
};
