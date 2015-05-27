#pragma once
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QStandardItem>
#include <QtGui/QListView>
#include "../DB/Retriever.h"
#include "../Utils/Utilities.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <map>

class ListHandle
{
public:
	ListHandle(void);
	~ListHandle(void);

	void moveSelectedItemWithDelete(QListView* source, QStandardItemModel* sourceModel, QListView* destination, QStandardItemModel* destinationModel);
	int addItemsFromDB(QListView* source, QStandardItemModel* model, vector<string> itemsFromDB, char prefix, string query);
	int addItemsFromDB(QListView* source, QStandardItemModel* model, vector<string> itemsFromDB, string query);
	int addItems(QListView* source, QStandardItemModel* model, string name);
	int addItems(QListView* source, QStandardItemModel* model, vector<string> names);
	map<string, vector<string>>  ListHandle::getItems(vector<string> itemsFromDB, string query);
	vector<string> getConcatenatedText(map<string, vector<string>> dataContainer, vector<string> itemsFromDB);
	vector<string> getSelectedItemTextList(QListView* source, int idx);
	vector<string> getSelectedItemTextList(QListView* source, QStandardItemModel* sourceModel);
	string getSelectedItemText(QModelIndex index);
	string getSelectedItemText(QModelIndex index, int idx);
	QModelIndex getSelectedItemIndex(QListView* source, string text);
	void removeAll(QListView* source);
	vector<string> getConcatenatedText(map<string, vector<string>> dataContainer, vector<string> itemsFromDB, char prefix);

};
