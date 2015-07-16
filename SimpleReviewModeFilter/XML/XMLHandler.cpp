#include "../stdafx.h"
#include "XMLHandler.h"
#include "../Utils/Utilities.h"

using namespace rapidxml;
using namespace std;

static xml_node<>* foundnode = NULL;
static xml_node<>* config_node = NULL;

XMLHandler::XMLHandler(string filename)
{
	this->filename = filename;
}

XMLHandler::~XMLHandler(void)
{

}

void XMLHandler::open(){
	MagnaUtil::delete_file(this->filename);
	tmpFile.open(filename.c_str());
}

void XMLHandler::addHeader(string root_name)
{
	if (tmpFile.is_open())
	{
		tmpFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << endl;
		tmpFile << "<" << root_name.c_str() << ">" << endl;
	}
}

void XMLHandler::addItems(string item_name, string sub_item_node, vector<string> sub_items)
{
	tmpFile << "	<" << item_name.c_str() << "> " << endl;
	for(unsigned int idx = 0 ; idx < sub_items.size() ; idx++)
	{
		tmpFile << "		<" << sub_item_node.c_str() << ">";
		tmpFile << sub_items.at(idx).c_str();
		tmpFile << "</" << sub_item_node.c_str() << ">" << endl;
	}
	tmpFile << "	</" << item_name.c_str() << "> " << endl;
}

void XMLHandler::addItem(string item_name, string sub_item_att_name, string sub_item_att_value)
{
	tmpFile << "	<" << item_name.c_str() << " " << sub_item_att_name.c_str() <<" = '" << sub_item_att_value.c_str() << "'/>" << endl;
}

void XMLHandler::addItem(string item_name, string sub_item_att_name, bool sub_item_att_value)
{
	tmpFile << "	<" << item_name.c_str() << " " << sub_item_att_name.c_str() <<" = '" << MagnaUtil::boolToString(sub_item_att_value) << "'/>" << endl;
}

void XMLHandler::addFooter(string root_name)
{
	tmpFile << "</" << root_name.c_str() << ">" << endl;
}

void XMLHandler::close()
{
	tmpFile.close();
}

vector<string> XMLHandler::getNodeListAtFirstLevel(string node_name)
{
	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	vector<string> found_node_list;
	xml_node<>* root = this->doc.first_node(node_name.c_str());

	for (xml_node<> *child = root->first_node(); child; child = child->next_sibling()) 
	{
		string cur_node_name = child->name();
		found_node_list.push_back(child->value());
	}

	return found_node_list;
}

vector<string> XMLHandler::getNodeNameListAtFirstLevel(string node_name)
{
	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	vector<string> found_node_list;
	xml_node<>* root = this->doc.first_node(node_name.c_str());

	for (xml_node<> *child = root->first_node(); child; child = child->next_sibling()) 
	{
		string cur_node_name = child->name();
		found_node_list.push_back(cur_node_name);
	}

	return found_node_list;
}

vector<string> XMLHandler::getNodeListAtSecondLevel(string node_name)
{
	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	vector<string> found_node_list;
	xml_node<>* root = this->doc.first_node("backup")->first_node(node_name.c_str());

	for (xml_node<> *child = root->first_node(); child; child = child->next_sibling()) 
	{
		string cur_node_name = child->name();
		found_node_list.push_back(child->value());
	}

	return found_node_list;
}

void XMLHandler::removeNodeAtSecondLevel(string node_name, string node_content)
{
	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	vector<string> found_node_list;
	xml_node<>* root = this->doc.first_node("backup")->first_node(node_name.c_str());

	for (xml_node<> *child = root->first_node(); child; child = child->next_sibling()) 
	{
		string cur_node_name = child->name();
		found_node_list.push_back(child->value());
		string cur_content(child->value());
		if(cur_content.compare(node_content)==0)
		{
			root->remove_node(child);
			break;
		}
	}
	this->save();
}


string XMLHandler::getNodeValue(string node_name, string att_name)
{
	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	vector<string> found_node_list;
	xml_node<>* root = this->doc.first_node("backup")->first_node(node_name.c_str());
	string attribute_value;
	for (xml_attribute<> *attribute = root->first_attribute(); attribute; attribute = attribute->next_attribute()) {
		string cur_att_name = attribute->name();
		string cur_att_value = attribute->value();

		if(cur_att_name.compare(att_name)==0)
		{
			attribute_value = cur_att_value;
		}
	}

	return attribute_value;
}

string XMLHandler::getCurDatFile(string path1, string path2, string mode_name, string filter_name, string filter_value){

	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	xml_node<>* root = this->doc.first_node(path1.c_str())->first_node(path2.c_str());
	getConfiguration(root, mode_name);
	if(config_node==NULL)
	{
		MagnaUtil::show_message("system_release.xml file error \n in finding Review Configuration.");
		return "";
	}

	string t = "";
	getNodeValue(config_node, filter_name, filter_value);

	if(foundnode!=NULL){
		cout << foundnode->first_attribute()->value() << endl;
		string cur_att_value = foundnode->first_attribute("value")->value();
		t = cur_att_value;
	}

	return t;
}


void XMLHandler::update(string path1, string path2, string mode_name, string filter_name, string filter_value, string replace_value){

	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	xml_node<>* root = this->doc.first_node("adtf:project")->first_node("configurations");
	getConfiguration(root, mode_name);
	if(config_node==NULL)
	{
		MagnaUtil::show_message("system_release.xml file error \n in finding Review Configuration!");
		return;
	}

	getNodeValue(config_node, filter_name, filter_value);

	if(foundnode!=NULL){
		xml_attribute<> *attribute = foundnode->first_attribute("value");
		attribute->value(replace_value.c_str());
		this->save();
	}
}

vector<string> XMLHandler::getConfigurationNames(string path1, string path2){

	ifstream theFile (filename.c_str());
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	this->doc.parse<0>(&buffer[0]);
	xml_node<>* root_node = this->doc.first_node(path1.c_str())->first_node(path2.c_str());
	vector<string> configurations;

	for (xml_node<> *child = root_node->first_node(); child; child = child->next_sibling()) 
	{
		string node_name = child->name();
		for (xml_attribute<> *attribute = child->first_attribute(); attribute; attribute = attribute->next_attribute()) 
		{
			string att_name = attribute->name();
			string att_value = attribute->value();
			if(node_name.compare("configuration")==0 && att_name.compare("name")==0){
				configurations.push_back(att_value);
			}
		}
	}

	return configurations;
}


void XMLHandler::getConfiguration(xml_node<> * root_node, string config_name)
{
	for (xml_node<> *child = root_node->first_node(); child; child = child->next_sibling()) 
	{
		for (xml_attribute<> *attribute = child->first_attribute(); attribute; attribute = attribute->next_attribute()) 
		{
			string att_name = attribute->name();
			string att_value = attribute->value();
			if(att_name.compare("name")==0 && att_value.compare(config_name) == 0){
				config_node = child;
				return;
			}
		}
	}
}
void XMLHandler::getNodeValue(xml_node<> * root_node, string filter_name, string filter_value){
	bool bln_att_file_name=false;
	bool bln_att_dat_node =false;
	bool bln_parent = false;
	xml_node<> *hard_disk_player_node = NULL;

	for (xml_node<> *child = root_node->first_node()->first_node(); child; child = child->next_sibling()) {
		for (xml_attribute<> *child_attribute = child->first_attribute(); child_attribute; child_attribute = child_attribute->next_attribute()) {
			string att_name_property = child_attribute->name();
			string att_value_property = child_attribute->value();
			if(att_name_property.compare("idref")==0 && att_value_property.compare("adtf.stg.harddisk_player")==0) {
				hard_disk_player_node = child;
				break;
			}
		}
	}

	if(hard_disk_player_node==NULL) {
		MagnaUtil::show_message("system_release.xml file error \n in finding Hard_Disk_Player in Review Configuration.");
		return;
	}

	for (xml_node<> *child = hard_disk_player_node->first_node()->first_node(); child; child = child->next_sibling()) {
		for (xml_attribute<> *child_attribute = child->first_attribute(); child_attribute; child_attribute = child_attribute->next_attribute()) {
			string att_name_property = child_attribute->name();
			string att_value_property = child_attribute->value();
			if(att_name_property.compare("name")==0 && att_value_property.compare("filename")==0) {
				foundnode = child;
				break;
			}
		}
	}
}

void XMLHandler::save()
{
	std::ofstream file_stored(this->filename.c_str());
	file_stored << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
	file_stored << doc;
	file_stored.close();
	doc.clear();
}