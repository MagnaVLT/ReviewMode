
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

string QueryFactory::getAnnotationCategoryQuery(vector<string> featureList, string projectid)
{

	if(featureList.size()==0){
		return "";
	}

	string query = "select name, id from predefined_annotation_list ";

	query += " where id <> 0 ";
	if(featureList.size()>0)
	{
		query += " and (";
		for(unsigned int i =0 ; i < featureList.size()-1 ; i++)
		{
			query+= "featureid = '" + featureList.at(i) + "' or ";
		}
		query += " featureid = '" + featureList.at(featureList.size()-1) + "') ";
	}

	query += " and projectid = " + projectid;
	query += ";";

	return query;
}

string QueryFactory::getEventTypeQuery(vector<string> items, string table, vector<string> event_catetories, string stime, string etime, string projectid, vector<string> feature,
									   string vin, bool chk_tour, string start_clip, string end_clip)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from " + table;
	query += " where a.id = b.eventid and b.projectid = " + projectid + " ";
	if(!stime.empty() && !etime.empty())
		query+= " and date(b.localpctime) >= '" + stime + "' and date(b.localpctime) <= '" + etime + "' ";

	if(vin!= "")
	{
		query+= " and b.vin = '" + vin + "' ";
	}

	if(chk_tour == false && start_clip != "" && end_clip != "")
	{
		query+= " and b.clipid >= " + start_clip + " and b.clipid <= " + end_clip + " ";
	}

	query += " and a.id in (select d.id from project_event_map c, event_list d where c.eventid = d.id and c.projectid = "+projectid + " ";
	query = this->addFields("d.featureid", feature, query);
	query+= ")";
	query = this->addFields("b.eventcategoryid", event_catetories, query);
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


string QueryFactory::getEventListQuery(int offset, vector<string> items, string userid, string projectid, vector<string> events, 
									   string stime, string etime, vector<string> event_categories, vector<string> predefined_annotation, string search_condition, bool chk_search,
									   string vin, bool chk_tour, string start_clip, string end_clip)
{
	string query = "";
	query+= "select ";
	for (unsigned int i =0 ; i < items.size()-1 ; i++)	{
		query+= items.at(i) + ", ";
	}
	query+=  items.at(items.size()-1) + " " ;

	query += " from event_report a, event_list b where a.eventid = b.id ";
	query += " and projectid = " + projectid;
	
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

	if(vin!= "")
	{
		query+= " and a.vin = '" + vin + "' ";
	}

	if(chk_tour == false && start_clip != "" && end_clip != "")
	{
		query+= " and a.clipid >= " + start_clip + " and a.clipid <= " + end_clip + " ";
	}

	query+= " order by reportid desc limit " + MagnaUtil::integerToString(offset) + ", 100;";

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