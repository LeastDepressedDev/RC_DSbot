#pragma once
#include <iostream>
#include <dpp/dpp.h>

namespace tte {
	class tte_session {
	public:
		dpp::snowflake p1;
		dpp::snowflake p2;
		dpp::snowflake turn_p;
		std::pair<dpp::snowflake, dpp::snowflake> position;
		dpp::snowflake control_msg_id;
		std::string desk[3][3];
		//in pair<snowflake, snowflake> first pos is guild id, second is channel id
		tte_session(std::pair<dpp::snowflake, dpp::snowflake> position, dpp::snowflake p1, dpp::snowflake p2);
		void start();
		void update();
		void act(dpp::snowflake, int, int);
		void freeze();
		void unfreeze();
		void leave(dpp::snowflake);
		void end(dpp::snowflake);
		void end();
	private:
		int i_turn_c = 0;
		void outline(char c);
		void swp_player();
		dpp::cluster* controller;
		bool frozen = false;
		bool turn = false;
		bool reversed = true;
		bool ended = false;
	};
}