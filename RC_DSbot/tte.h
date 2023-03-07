#pragma once
#include <iostream>
#include "tte_session.h"
#include <vector>

namespace tte {
	extern std::vector<tte_session*> sessions;

	extern void add(tte_session* ses);
	extern void enclose(std::pair<dpp::snowflake, dpp::snowflake> ses_id_as_pair);
	extern void button_ofc(dpp::snowflake who_clicked, std::string button_id);
}