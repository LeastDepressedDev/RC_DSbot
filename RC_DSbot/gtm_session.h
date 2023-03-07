#pragma once
#include <dpp/dpp.h>

namespace gtm {
	class gtm_session {
	public:
		dpp::snowflake ch_id;
		dpp::snowflake guild_id;

		gtm_session(size_t, dpp::snowflake*);
		void start();
	private:
		dpp::cluster* controller;
		dpp::snowflake spec_ch();
		unsigned short turn;
		size_t ucount;
		dpp::snowflake* u_list;
	};
}