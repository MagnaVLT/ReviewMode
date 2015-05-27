#include <string>
#include <map>
#include <vector>

using namespace std;

//typedef struct
//{
//	map<string, vector<string>> containers;
//	vector<string> item_list;
//
//}sEvent_Data;


typedef struct
{
	string report_id;
	string vin;
	string company_id;
	string clip_id;
	string event_id;
	string event_status_id;
	string event_category_id;
	string local_pc_time;
	string adtf_time;
	string user_annotation;
	string gps_longitude;
	string gps_latitude;
	string grap_index;
	string predefined_annotation_id;


	string country_code_id;
	string day_type_id;
	string weather_type_id;
	string road_type_id;
	string clip_name;
	string start_time;
	string stop_time;
	string local_pc_time_clip;
	string fcm_sw_ver;
	string mest_ver;
	string adtf_ver;
	string driver_name;
	string gps_timestamp;
	string gps_longitude_clip;
	string gps_latitude_clip;
	string gps_speed;
	string gps_heading;
	string gps_invalid_counter;
	string gps_elevation;
	string recording_purpose;


}sEvent_Data;