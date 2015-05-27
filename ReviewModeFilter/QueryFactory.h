#pragma once
#include <string>
#include <vector>

using namespace std;

class QueryFactory
{
public:
	QueryFactory(void);
	~QueryFactory(void);
	string getAnnotationCategoryQuery(vector<string> conditions, string projectid);
	string getEventTypeQuery(vector<string> items, string table, vector<string> event_types, string stime, string etime, string projectid, vector<string> feature);
	string getEventListQuery(vector<string> items, string userid, string projectid, vector<string> events, 
		string stime, string etime, vector<string> event_categories, vector<string> predefined_annotation, string search_condition, bool chk_search);

private:
	string addFields(string field, vector<string> values, string query);
	void addGroupByClaud(vector<string> items, string* query);
};