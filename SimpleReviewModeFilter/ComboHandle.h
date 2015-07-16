#pragma once
#include <string>
#include <vector>
#include <QtGui/QComboBox>

using namespace std;

class ComboHandle
{
public:
	ComboHandle(void);
	~ComboHandle(void);
	void initCombo(QComboBox* combobox, vector<string> data);
};
