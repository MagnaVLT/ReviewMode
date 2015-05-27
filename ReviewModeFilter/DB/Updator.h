#pragma once
#include <vector>
#include <string>
#include "DataHandler.h"

using namespace std;

class Updator: public DataHandler
{
public:
	Updator();
	Updator(string query);
	void setQuery();
	void execute();
	void execute(string query);
	~Updator(void);

private:
};

