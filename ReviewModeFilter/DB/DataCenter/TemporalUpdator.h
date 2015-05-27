#pragma once
#include <vector>
#include <string>
#include "TemporalDataHandler.h"

using namespace std;

class TemporalUpdator: public TemporalDataHandler
{
public:
	TemporalUpdator();
	TemporalUpdator(string query);
	void setQuery();
	void execute();
	void execute(string query);
	~TemporalUpdator(void);

private:
};

