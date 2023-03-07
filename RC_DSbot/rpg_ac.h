#pragma once
#include "riki_rpg.h"

namespace rpg {
	class User {
	public:
		dpp::snowflake get_uid();
		User(dpp::snowflake);
	private:
		dpp::snowflake user_id;
	};

	extern std::string token_gen();
}