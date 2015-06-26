#pragma once
#include <string>
#include <vector>

using namespace std;

class QueryFactory
{
public:
	QueryFactory(void);
	~QueryFactory(void);
	string getAnnotationCategoryQuery(vector<string> conditions, vector<string> projectid);
	std::string getEventTypeQuery(vector<string> items, string table, vector<string> event_types, string stime, string etime, vector<string> project_id, vector<string> feature,
		vector<string> vin, bool chk_tour, string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads, vector<string> status);
	string getEventListQuery(int offset, vector<string> items, string userid, vector<string> projectid, vector<string> events, 
		string stime, string etime, vector<string> event_categories, vector<string> predefined_annotation, string search_condition, bool chk_search,
		vector<string> vin, bool chk_tour, string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads, vector<string> event_status, vector<string> ai_types, vector<string> ai_values, bool isEvent);
	string addEventReportTable(vector<string> ai_types, vector<string> ai_values);
	void addTypeAndValuePair(string* query, vector<string> ai_types, vector<string> ai_value);
	std::string addFieldsViaInStatement(string field, vector<string> values, string query, int order, bool is_text);

private:
	string addFields(string field, vector<string> values, string query);
	void addGroupByClaud(vector<string> items, string* query);
};