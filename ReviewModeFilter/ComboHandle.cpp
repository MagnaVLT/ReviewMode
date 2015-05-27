#include "ComboHandle.h"

ComboHandle::ComboHandle(void)
{
}

ComboHandle::~ComboHandle(void)
{
}


void ComboHandle::initCombo(QComboBox* combobox, vector<string> data)
{	
	combobox->addItem(QString(""));
	for(unsigned int i = 0 ; i < data.size() ; i++){
		string a = data.at(i);
		if(a.empty()==false)
			combobox->addItem(QString(a.c_str()));
	}
}