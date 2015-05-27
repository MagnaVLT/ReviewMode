#pragma once
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
using namespace rapidxml;

class XMLHandler
{
public:
	XMLHandler(string filename);
	~XMLHandler(void);
	void update(string path1, string path2, string filter_name, string filter_value, string replace_value);
	string getCurDatFile(string path1, string path2, string filter_name, string filter_value);
	vector<string> getNodeListAtSecondLevel(string node_name);
	vector<string> getNodeListAtFirstLevel(string node_name);
	vector<string> getNodeNameListAtFirstLevel(string node_name);
	string getNodeValue(string node_name, string att_name);

	void open();
	void addHeader(string root_name);
	void addItems(string item_name, string sub_item_node, vector<string> sub_items);
	void addItem(string item_name, string sub_item_att_name, string sub_item_att_value);
	void addItem(string item_name, string sub_item_att_name, bool sub_item_att_value);
	void addFooter(string root_name);
	void close();
	void addNode(string node_name, string node_content);
	void removeNodeAtSecondLevel(string node_name, string node_content);

private:
	//xml_node<>* getNode(string node_name);
	void getNodeValue(xml_node<> * root_node, string att_name, string att_value);
	void save();
	string filename;
	ofstream tmpFile;
	xml_document<> doc;
};