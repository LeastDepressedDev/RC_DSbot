#pragma once
#include <dpp/dpp.h>
#include <map>

namespace gtm {

	extern enum e_stage : uint8_t {
		theme_pick = 0,
		selecting_react = 1,
		guessing = 2,
		extra_guessing = 3
	};

	class gtm_session {
	public:
		dpp::snowflake ch_id;
		dpp::snowflake guild_id;
		std::map<dpp::snowflake, dpp::snowflake> linker;

		gtm_session(dpp::snowflake, dpp::snowflake, size_t, dpp::snowflake*);
		gtm_session() {}
		void start();
		void ex_task(dpp::snowflake sender, const dpp::message_create_t event);
	private:
		bool paused = false;
		void pause();
		void resume();

		void game_proc();
		e_stage e_game;

		std::string s_theme;

		dpp::snowflake w_msgId;
		dpp::cluster* controller;
		void theme_pick(std::string);
		void proc_m();
		void spec_ch();
		void c_update();

		void fbd_con(dpp::snowflake);
		void agr_con(dpp::snowflake);
		//user id and his channel id
		
		unsigned short turn;
		size_t ucount;
		dpp::snowflake* u_list;

		//theme_pick
		//
	};
}