#pragma once
#include <iostream>
#include <vector>
#include "gtm_session.h"

namespace gtm {

	extern std::vector<std::string> resources;

	extern std::vector<gtm::gtm_session*> sessions;
	extern std::vector<dpp::snowflake> active_players;

	extern bool regNewPlayer(dpp::snowflake);
	extern bool unregPlayer(dpp::snowflake);

	extern bool hasChannel(dpp::snowflake);
	extern gtm::gtm_session* getPlSes(dpp::snowflake);
	extern gtm::gtm_session* getChSes(dpp::snowflake);

	extern void regGame(gtm::gtm_session*);
	extern void closeGame(gtm::gtm_session);

	extern void __init(std::map<std::string, std::string> &loc);

	inline const std::string sDescr = "";
}