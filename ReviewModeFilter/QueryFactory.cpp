
#include "stdafx.h"
#include "QueryFactory.h"
#include <QtGui/QMessageBox>
#include "Utils/Utilities.h"

QueryFactory::QueryFactory(void)
{
}

QueryFactory::~QueryFactory(void)
{
}

std::string QueryFactory::getAnnotationCategoryQuery(vector<string> conditions, vector<string> projectid)
{

	if(conditions.size()==0){
		return "";
	}

	string query = "select name, id from predefined_annotation_list ";

	query += " where id <> 0 ";
	if(conditions.size()>0)
	{
		query += " and (";
		for(unsigned int i =0 ; i < conditions.size()-1 ; i++)
		{
			query+= "featureid = '" + conditions.at(i) + "' or ";
		}
		query += " featureid = '" + conditions.at(conditions.size()-1) + "') ";
	}
	query = this->addFieldsViaInStatement("projectid", projectid, query, 2, false);
	query += ";";

	return query;
}

std::string QueryFactory::getEventTypeQuery(vector<string> items, string table, vector<string> event_types, string stime, string etime, 
											vector<string> feature, vector<string> project_id,
									   vector<string> vin, bool chk_tour, string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads, vector<string> status)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from ";
	query += "(select d.name as name, d.id as id from project_event_map c, event_list d where c.eventid = d.id ";
	query = this->addFieldsViaInStatement("c.projectid", project_id, query, 2, false);
	query = this->addFieldsViaInStatement("d.featureid", feature, query, 2, false);
	query+= ") a, " + table ;
	query += " where a.id = b.eventid ";
	query += " and b.clipid = c.clipid ";

	query = this->addFieldsViaInStatement("b.projectid", project_id, query, 2, false);
	if(!stime.empty() && !etime.empty())
		query+= " and date(b.localpctime) >= '" + stime + "' and date(b.localpctime) <= '" + etime + "' ";

	query = this->addFieldsViaInStatement("b.vin", vin, query, 2, true);
	query = this->addFieldsViaInStatement("c.daytypeid", days, query, 2, false);
	query = this->addFieldsViaInStatement("c.weathertypeid", weathers, query, 2, false);
	query = this->addFieldsViaInStatement("c.roadtypeid", roads, query, 2, false);

	if(chk_tour == false && start_clip != "" && end_clip != "")
	{
		query+= " and b.clipid >= " + start_clip + " and b.clipid <= " + end_clip + " ";
	}


	query = this->addFieldsViaInStatement("b.eventcategoryid", event_types, query, 2, false);
	query = this->addFieldsViaInStatement("b.eventstatusid", status, query, 2, false);

	this->addGroupByClaud(items, &query);

	return query;
}


void QueryFactory::addGroupByClaud(vector<string> items, string* query)
{
	*query += " group by ";

	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		*query+= items.at(i) + ", ";
	}

	*query+=  items.at(items.size()-1) + " " ;

	*query+= " order by ";
	for (unsigned int i =items.size()-1 ; i > 0 ; i--)	{
		*query+= items.at(i) + ", ";
	}

	*query+=  items.at(0);
}

string QueryFactory::getEventListQuery(int offset, vector<string> items, string userid, vector<string> projectid, vector<string> events, 
									   string stime, string etime, vector<string> event_categories, vector<string> predefined_annotation, string search_condition, bool chk_search,
									   vector<string> vin, bool chk_tour, string start_clip, string end_clip, 
									   vector<string> days, vector<string> weathers, vector<string> roads, vector<string> event_status, vector<string> ai_types, vector<string> ai_values, bool isEvent)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from ";
	if(!ai_types.empty() && (ai_values.size() == ai_types.size()))
	{
		query +=  + "(" + this->addEventReportTable(ai_types, ai_values) + ") a, ";
	}else
	{
		query += " event_report a, ";
	}
	
	query += " event_list b, clip_info c where a.eventid = b.id and a.clipid = c.clipid ";
	query = this->addFieldsViaInStatement("projectid", projectid, query, 2, false);
	query = this->addFieldsViaInStatement("c.daytypeid", days, query, 2, false);
	query = this->addFieldsViaInStatement("c.weathertypeid", weathers, query, 2, false);
	query = this->addFieldsViaInStatement("c.roadtypeid", roads, query, 2, false);
	query = this->addFieldsViaInStatement("a.eventstatusid", event_status, query, 2, false);
	query = this->addFieldsViaInStatement("a.eventcategoryid ", event_categories, query, 2, false);
	query = this->addFieldsViaInStatement("a.eventid ", events, query, 2, false);

	if(chk_search)
	{
		if(!search_condition.empty())
			query += " and a.userannotation like '%" + search_condition + "%'";
		else
			query += " and a.userannotation = ''";
	}else 
	{
		if(predefined_annotation.size()>0)
			query = addFields(" a.predefinedannotationid ", predefined_annotation, query);
	}
	
	if(!stime.empty() && !etime.empty())
	{
		query+= " and date(a.localpctime) >= '" + stime + "' and date(a.localpctime) <= '" + etime + "' ";
	}

	query = this->addFieldsViaInStatement("a.vin", vin, query, 2, true);

	if(chk_tour == false && start_clip != "" && end_clip != "")
	{
		query+= " and a.clipid >= " + start_clip + " and a.clipid <= " + end_clip + " ";
	}

	if(isEvent==true) query+= " order by reportid desc limit " + MagnaUtil::integerToString(offset) + ", 100;";
	else query += " group by clipname;";

	return query;
}

std::string QueryFactory::addEventReportTable(vector<string> ai_types, vector<string> ai_values)
{
	string query = "select d.* ";
	query += " from event_report d, (";
	query += " select g.reportid, g.vin from additional_event_report g where ";
	this->addTypeAndValuePair(&query, ai_types, ai_values);	
	query +=") f";
	query += " where d.reportid = f.reportid and d.vin = f.vin ";
	return query;
}

void QueryFactory::addTypeAndValuePair(string* query, vector<string> ai_types, vector<string> ai_value)
{
	for(unsigned int i = 0 ; i < ai_types.size()-1 ; i++)
	{
		*query += " (typeid = " + ai_types.at(i) + " and value = " + ai_value.at(i) + ") or";
	}
	*query += " (typeid = " + ai_types.at(ai_value.size()-1) + " and value = " + ai_value.at(ai_value.size()-1) + ")";
}


string QueryFactory::addFields(string field, vector<string> values, string query)
{
	if(values.size()>0)
	{
		query += " and (";
			for(unsigned int i = 0 ; i < values.size()-1 ; i++)
			{
				string v = values.at(i);
				query += field + " = " + v + " or ";
			}
			string v = values.at(values.size()-1);
			query += field + " = " + v + ") ";

	}else{
		query += " and " + field + " = -1 ";
	}	
	
	return query;
}



std::string QueryFactory::addFieldsViaInStatement(string field, vector<string> values, string query, int order, bool is_text)
{
	if(values.size()>0)
	{
		if(order == 1)query += " where ";
		else query += " and ";
		query += " " + field + " in (";
		if(is_text==false)
		{
			for(unsigned int i = 0; i < values.size()-1; i++) query+= values.at(i) + ", " ;
			query += values.at(values.size()-1);
		}else{
			for(unsigned int i = 0; i < values.size()-1; i++) query+= "'" + values.at(i) + "', " ;
			query +=  "'"+values.at(values.size()-1) + "'";
		}


		query += ") ";
	}


	return query;
}

