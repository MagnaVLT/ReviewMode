#pragma once
#include <vector>
#include <string>
#include "TempDBMakeDataHandler.h"

using namespace std;

class TempDBMakeUpdator: public TempDBMakeDataHandler
{
public:
	TempDBMakeUpdator();
	TempDBMakeUpdator(string query);
	void setQuery();
	void execute();
	void execute(string query);
	~TempDBMakeUpdator(void);

private:
};

