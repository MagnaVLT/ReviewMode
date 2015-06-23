
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

std::string QueryFactory::getEventTypeQuery(vector<string> items, string table, vector<string> event_types, string stime, string etime, vector<string> projectid, vector<string> feature,
									   vector<string> vin, bool chk_tour, string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from " + table;
	query += " where a.id = b.eventid ";
	query += " and b.clipid = c.clipid ";

	query = this->addFieldsViaInStatement("b.projectid", projectid, query, 2, false);
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

	query += " and a.id in (select d.id from project_event_map c, event_list d where c.eventid = d.id ";
	query = this->addFieldsViaInStatement("c.projectid", projectid, query, 2, false);
	query = this->addFields("d.featureid", feature, query);
	query+= ")";
	query = this->addFields("b.eventcategoryid", event_types, query);
	this->addGroupByClaud(items, &query);

	LOG_INFO(query.c_str());

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


std::string QueryFactory::getEventListQuery(int offset, vector<string> items, string userid, vector<string> projectid, vector<string> events, 
									   string stime, string etime, vector<string> event_categories, vector<string> predefined_annotation, string search_condition, bool chk_search,
									   vector<string> vin, bool chk_tour, string start_clip, string end_clip, vector<string> days, vector<string> weathers, vector<string> roads, vector<string> event_status)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from event_report a, event_list b, clip_info c where a.eventid = b.id and a.clipid = c.clipid ";
	query = this->addFieldsViaInStatement("projectid", projectid, query, 2, false);
	query = this->addFieldsViaInStatement("c.daytypeid", days, query, 2, false);
	query = this->addFieldsViaInStatement("c.weathertypeid", weathers, query, 2, false);
	query = this->addFieldsViaInStatement("c.roadtypeid", roads, query, 2, false);
	query = this->addFieldsViaInStatement("a.eventstatusid", event_status, query, 2, false);
	
	query = addFields(" a.eventcategoryid ", event_categories, query);
	query = addFields(" a.eventid ", events, query);

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

	query+= " order by reportid desc limit " + MagnaUtil::integerToString(offset) + ", 100;";

	LOG_INFO(query.c_str());
	return query;
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

