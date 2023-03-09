#pragma once
#include <iostream>
#include <vector>
#include "gtm_session.h"

namespace gtm {
	extern std::vector<gtm::gtm_session> sessions;
	extern std::vector<dpp::snowflake> active_players;

	extern bool regNewPlayer(dpp::snowflake);
	extern bool unregPlayer(dpp::snowflake);

	extern bool hasChannel(dpp::snowflake);

	extern void regGame(gtm::gtm_session);
	extern void closeGame(gtm::gtm_session);

	inline const std::string sDescr = "";
}