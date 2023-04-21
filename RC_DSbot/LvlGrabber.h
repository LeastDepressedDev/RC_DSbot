#pragma once
#include <dpp/dpp.h>
#include <map>
#include "LvlProcessor.h"

#define DIR_LVL_GUILD_EXTENSION ".lvl"
#define SAVE_REQ 10

class LvlGrabber {
public:

	size_t sizing = 0;
	
	std::string pth;

	//guild id - map: uid - stats
	std::map<dpp::snowflake, std::map<dpp::snowflake, UserStat>> mp;

	LvlGrabber();

	void upt();
	void read();
	void grab(dpp::snowflake guild, dpp::snowflake uid, std::string msg);

	void display(dpp::snowflake guild, dpp::snowflake uid, dpp::snowflake chid);

private:

	void newA(dpp::snowflake, dpp::snowflake);
};