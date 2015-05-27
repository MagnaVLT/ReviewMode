#include "../stdafx.h"
#include "ListHandle.h"
#include <sstream>
#include <QtCore/QString>
#include <QtGui/QMessageBox>

ListHandle::ListHandle(void)
{

}

ListHandle::~ListHandle(void)
{
}

void ListHandle::moveSelectedItemWithDelete(QListView* source, QStandardItemModel* sourceModel, QListView* destination, QStandardItemModel* destinationModel)
{


	for each(const QModelIndex &index in source->selectionModel()->selectedIndexes())
	{
		QStandardItem* items = new QStandardItem(sourceModel->itemFromIndex(index)->text());
		destinationModel->appendRow(items);
	}

	destination->setModel(destinationModel);

	QModelIndexList indexes = source->selectionModel()->selectedIndexes();
	while(indexes.size()) {
		sourceModel->removeRow(indexes.first().row());
		source->setModel(sourceModel);
		indexes = source->selectionModel()->selectedIndexes();
	}

	//source->setModel(sourceModel);

}

void ListHandle::removeAll(QListView* source){

	
	for (int i = 0 ; i < source->model()->rowCount() ; i++)
	{
		source->model()->removeRow(i);
	}
}

vector<string> ListHandle::getSelectedItemTextList(QListView* source, int idx)
{
	vector<string> fids;

	for (int i = 0 ; i < source->model()->rowCount() ; i++)
	{
		string temp = source->model()->index(i,0).data(Qt::DisplayRole).toString().toStdString();
		if(temp!=""){
			fids.push_back(MagnaUtil::stringTokenizer(temp, '-').at(idx));
		}
	}
	return fids;
}

vector<string> ListHandle::getSelectedItemTextList(QListView* source, QStandardItemModel* sourceModel)
{
	vector<string> selectedItem;
	for each(const QModelIndex &index in source->selectionModel()->selectedIndexes())
	{
		QStandardItem* items = new QStandardItem(sourceModel->itemFromIndex(index)->text());
		selectedItem.push_back(items->text().toStdString());
	}

	return selectedItem;
}


QModelIndex ListHandle::getSelectedItemIndex(QListView* source, string text)
{
	QModelIndex idx;
	for (int i = 0 ; i < source->model()->rowCount() ; i++)
	{
		string temp = source->model()->index(i,0).data(Qt::DisplayRole).toString().toStdString();
		if(temp.compare(text)==0){
			idx = source->model()->index(i,0);
			break;
		}
	}
	
	return idx;
}
string ListHandle::getSelectedItemText(QModelIndex index, int idx)
{
	string value = "";
	value = index.data().toString().toStdString();
	value = MagnaUtil::stringTokenizer(value, '-').at(idx);
	return value;
}

string ListHandle::getSelectedItemText(QModelIndex index)
{
	string value = "";
	value = index.data().toString().toStdString();
	return value;
}

int ListHandle::addItemsFromDB(QListView* source, QStandardItemModel* model, vector<string> itemsFromDB, string query){

	if(itemsFromDB.size()<1)
	{
		return 0;
	}

	map<string, vector<string>> dataContainer = (new Retriever(itemsFromDB, query))->getData();
	vector<string> names = getConcatenatedText(dataContainer, itemsFromDB);
	int count = this->addItems(source, model, names);

	return count;
}

int ListHandle::addItemsFromDB(QListView* source, QStandardItemModel* model, vector<string> itemsFromDB, char prefix, string query){

	if(itemsFromDB.size()<1)
	{
		return 0;
	}

	map<string, vector<string>> dataContainer = (new Retriever(itemsFromDB, query))->getData();
	vector<string> names = getConcatenatedText(dataContainer, itemsFromDB, prefix);
	int count = this->addItems(source, model, names);

	return count;
}


int ListHandle::addItems(QListView* source, QStandardItemModel* model, string name){

	source->setSelectionMode( QAbstractItemView::ExtendedSelection );
	int count = 1;
	bool chk = false;
	for (int i = 0 ; i < model->rowCount() ; i++ )
	{

		string temp = model->index(i,0).data(Qt::DisplayRole).toString().toStdString();
		if(temp.compare(name)==0)
		{
			QMessageBox b;
			b.setText(QString(temp.c_str()));
			b.exec();
			chk=true;
		}
	}

	if(!chk)
	{
		QStandardItem* items = new QStandardItem(QString(name.c_str()));
		model->appendRow(items);
	}

	source->setModel(model);
	return count;
}


int ListHandle::addItems(QListView* source, QStandardItemModel* model, vector<string> names){

	source->setSelectionMode( QAbstractItemView::ExtendedSelection );
	int count = 0;
	for each (string f in names)
	{
		count++;
		bool chk = false;
		for (int i = 0 ; i < model->rowCount() ; i++ )
		{

			string temp = model->index(i,0).data(Qt::DisplayRole).toString().toStdString();
			if(temp.compare(f)==0)
			{
				QMessageBox b;
				b.setText(QString(temp.c_str()));
				b.exec();
				chk=true;
			}
		}

		if(!chk)
		{
			QStandardItem* items = new QStandardItem(QString(f.c_str()));
			model->appendRow(items);
		}
		chk=false;
	}
	source->setModel(model);

	return count;
}

vector<string> ListHandle::getConcatenatedText(map<string, vector<string>> dataContainer, vector<string> itemsFromDB, char prefix)
{
	vector<string> text;

	if(itemsFromDB.size()<1){
		return text;
	}

	for(unsigned int i = 0 ; i < dataContainer[itemsFromDB.at(0)].size() ; i++)
	{
		stringstream s;
		s << prefix;
		string tmpText = s.str() + "-";
		for (unsigned int k=0; k < itemsFromDB.size() -1 ; k++)
		{
			string items = itemsFromDB.at(k);
			tmpText += dataContainer[items].at(i) + "-";
		}
		tmpText += dataContainer[itemsFromDB.at(itemsFromDB.size()-1)].at(i);
		text.push_back(tmpText);
	}

	return text;
}

vector<string> ListHandle::getConcatenatedText(map<string, vector<string>> dataContainer, vector<string> itemsFromDB)
{
	vector<string> text;
	
	if(itemsFromDB.size()<1){
		return text;
	}

	for(unsigned int i = 0 ; i < dataContainer[itemsFromDB.at(0)].size() ; i++)
	{
		string tmpText="";
		for (unsigned int k=0; k < itemsFromDB.size() -1 ; k++)
		{
			string items = itemsFromDB.at(k);
			tmpText += dataContainer[items].at(i) + "-";
		}
		tmpText += dataContainer[itemsFromDB.at(itemsFromDB.size()-1)].at(i);
		text.push_back(tmpText);
	}

	return text;
}


map<string, vector<string>>  ListHandle::getItems(vector<string> itemsFromDB, string query)
{	
	map<string, vector<string>> dataContainer = (new Retriever(itemsFromDB, query))->getData();
	return dataContainer;
}