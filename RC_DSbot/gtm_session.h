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
		std::map<dpp::snowflake, std::vector<std::string>> rsc_rs;
		size_t ucount;
		dpp::snowflake* u_list;

		gtm_session(dpp::snowflake, dpp::snowflake, size_t, dpp::snowflake*);
		gtm_session() {}
		void start();
		void ex_task(dpp::snowflake sender, const dpp::message_create_t event);
		void b_task(dpp::snowflake sender, const dpp::button_click_t event);
	private:
		bool paused = false;
		void pause();
		void resume();

		void r_rsctp();
		void game_proc();
		e_stage e_game;

		std::string s_theme;

		dpp::snowflake w_msgId;
		dpp::cluster* controller;
		
		void proc_m();
		void spec_ch();
		void c_update();

		void fbd_con(dpp::snowflake);
		void agr_con(dpp::snowflake);
		
		unsigned short turn;

		//theme_pick
		void theme_pick(std::string);

		//selecting react
		void prep_react();
		void sel_react(dpp::snowflake, uint16_t);

		//bool alr_reacted(dpp::snowflake);
		void proc_p2();
		std::map<dpp::snowflake, uint16_t> m_Lreact;


		//guessing
		void prep_guess();

		bool alr_voted(dpp::snowflake);
		std::map<dpp::snowflake, uint16_t> vote;
	};
}