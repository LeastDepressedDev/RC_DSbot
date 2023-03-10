#include "gtm.h"
#include "gtm_session.h"
#include "string_proc.h"
#include "g_rand.h"
#include "index.h"

const std::string WELCOME_MSG = "**Heya!**\nWelcome to the `guess the meme` game! Rules are pretty simple. At the game's start, players are getting 5 reaction cards. AI is randomly selecting a first driver. Driver is setting the situatuion(theme) for the other players.\nWhen theme is set, every player is selecting one reaction card from his resources. After that players are guessing the best reaction, if voices are equal, driver is selecting the winner. Winner is becoming the next driver.\nGame continues until last reaction card used.";
const std::string REACT_DEF_MSG = "**Theme set!**\nNow it's time to react\n";

gtm::gtm_session::gtm_session(dpp::snowflake gid, dpp::snowflake ch_id, size_t sz_t, dpp::snowflake* p_uList) {
	this->ucount = sz_t;
	this->ch_id = ch_id;
	this->guild_id = gid;
	this->turn = i_rand(0, sz_t);
	this->u_list = p_uList;
	this->linker = std::map<dpp::snowflake, dpp::snowflake>();
	this->rsc_rs = std::map<dpp::snowflake, std::vector<std::string>>();
	this->m_Lreact = std::map<dpp::snowflake, uint16_t>();
	this->vote = std::map<dpp::snowflake, uint16_t>();
}

void gtm::gtm_session::pause() {
	this->paused = true;
}

void gtm::gtm_session::resume() {
	this->paused = false;
}

void gtm::gtm_session::start() {
	this->controller = new dpp::cluster(APP.TOKEN, APP.INTS);
	this->w_msgId = this->controller->message_create_sync(dpp::message(this->ch_id, "Creating your game! Please wait!")).id;
	this->spec_ch();

	for (std::pair<dpp::snowflake, dpp::snowflake> pr : this->linker) {
		this->controller->message_create(dpp::message(pr.second, WELCOME_MSG));
	}

	this->controller->message_delete(this->w_msgId, this->ch_id);

	this->r_rsctp();

	gtm::regGame(this);
	this->proc_m();
}

void gtm::gtm_session::r_rsctp() {
	std::vector<int> used(0);
	for (int i = 0; i < this->ucount; i++) {
		dpp::snowflake id = this->u_list[i];
		std::vector<std::string> set(5);
		for (int j = 0; j < 5; j++) {
			int R = i_rand(0, gtm::resources.size());
			while ([R, used]() {
				for (int k : used) {
					if (R == k) return true;
				}
				return false;
				}())
			{
				R = i_rand(0, gtm::resources.size());
			}
			used.push_back(R);
			set[i] = R;
		}
		this->rsc_rs[id] = set;
	}
}

void gtm::gtm_session::spec_ch() {
	dpp::channel smp = dpp::channel();
	smp.set_type(dpp::channel_type::CHANNEL_TEXT);
	smp.name = "gtm_";
	smp.guild_id = this->guild_id;
	smp.add_permission_overwrite(this->guild_id, dpp::overwrite_type::ot_role, 0, dpp::permissions::p_view_channel);
	
	for (int i = 0; i < this->ucount; i++) {
		dpp::channel ch = smp;
		dpp::snowflake flake = this->u_list[i];
		ch.name = ch.name + std::to_string(flake);
		ch.add_permission_overwrite(flake, dpp::overwrite_type::ot_member, dpp::permissions::p_view_channel, dpp::permissions::p_send_messages);
		dpp::snowflake id = APP.BOT->channel_create_sync(ch).id;
		this->linker[flake] = id;
	}
}

void gtm::gtm_session::proc_m() {
	if (paused) return;
	dpp::snowflake uid = this->u_list[this->turn];
	dpp::snowflake ch_id = this->controller->channel_get_sync(this->linker[uid]).id;

	this->controller->message_create(dpp::message(ch_id, "**Your turn to pick a theme**\nJust write it in chat."));
	this->c_update();
}

void gtm::gtm_session::ex_task(dpp::snowflake sender, const dpp::message_create_t event) {
	std::string cont = event.msg.content;
	this->controller->message_delete(event.msg.id, event.msg.channel_id);
	if (paused) return;
	if (this->e_game == gtm::e_stage::theme_pick) {
		if (this->u_list[this->turn] == sender) {
			this->theme_pick(cont);
		}
	}
}

void gtm::gtm_session::b_task(dpp::snowflake sender, const dpp::button_click_t event) {
	if (paused) return;
	std::vector<std::string> cmd = str_proc::vec_split(event.custom_id, '_');
	if (cmd[1] == "sel") {
		this->sel_react(sender, std::stoi(cmd[2]));
	}
	else if (cmd[1] == "vt") {
		this->sel_react(sender, std::stoi(cmd[2]));
	}
}

void gtm::gtm_session::theme_pick(std::string theme) {
	if (paused) return;
	dpp::snowflake uid = this->u_list[this->turn];
	this->s_theme = theme;

	this->fbd_con(uid);
	this->game_proc();
}

void gtm::gtm_session::prep_react() {
	for (int i = 0; i < this->ucount; i++) {
		std::string line = REACT_DEF_MSG;
		dpp::snowflake ch_id = this->linker[this->u_list[i]];
		int j = 1;
		std::vector<std::string> t_rsc = this->rsc_rs[this->u_list[i]];
		for (std::string rs : t_rsc) {
			line += std::to_string(j) + rs + '\n';
			j++;
		}

		dpp::component but_sample = dpp::component();
		but_sample.set_type(dpp::component_type::cot_button);
		but_sample.set_style(dpp::component_style::cos_primary);

		dpp::message msg(ch_id, line);

		for (int o = 0; o < t_rsc.size(); o++) {
			dpp::component line = dpp::component();
			for (int h = 0; h < 5 && o < t_rsc.size(); h++) {
				std::string s_edit = std::to_string(o);
				line.add_component(but_sample.set_id("gtm_" + s_edit).set_label(s_edit));
			}
			msg.add_component(line);
		}

		this->controller->message_create(msg);
	}
}

//bool gtm::gtm_session::alr_reacted(dpp::snowflake usr) {
//	for (std::pair<dpp::snowflake, int> pr : this->m_Lreact) {
//		if (pr.first == usr) {
//			return true;
//		}
//	}
//	return false;
//}

void gtm::gtm_session::sel_react(dpp::snowflake sender, uint16_t i_react) {
	if (paused) return;
	if (this->e_game == gtm::e_stage::selecting_react) {
		//if (!alr_reacted(sender)) {
			this->m_Lreact[sender] = i_react;
		//}
	}
}

void gtm::gtm_session::proc_p2() {
	if (this->m_Lreact.size() == this->ucount) {
		this->pause();
		this->game_proc();
	}
}

void gtm::gtm_session::prep_guess() {
	//
	//
	//
	//  Place for a guessing message sending and blah-blah-blah
	//
	//
	//
	//
	//
	//
}

void gtm::gtm_session::game_proc() {
	switch (this->e_game)
	{
	case gtm::e_stage::theme_pick:
		this->e_game = e_stage::selecting_react;
		this->prep_react();
		break;
	case gtm::e_stage::selecting_react:
		this->e_game = e_stage::guessing;
		this->prep_guess();
		break;
	default:
		break;
	}
}

void gtm::gtm_session::c_update() {
	if (this->e_game == gtm::e_stage::theme_pick) {
		this->agr_con(this->u_list[this->turn]);
	}
	else {
		this->fbd_con(this->u_list[this->turn]);
	}
}

void gtm::gtm_session::fbd_con(dpp::snowflake usr) {
	dpp::snowflake ch_id = this->linker[usr];
	dpp::channel ch = this->controller->channel_get_sync(ch_id);
	ch.add_permission_overwrite(usr, dpp::overwrite_type::ot_member, 0, dpp::permissions::p_send_messages);
	this->controller->channel_edit(ch);
}

void gtm::gtm_session::agr_con(dpp::snowflake usr) {
	dpp::snowflake ch_id = this->linker[usr];
	dpp::channel ch = this->controller->channel_get_sync(ch_id);
	ch.add_permission_overwrite(usr, dpp::overwrite_type::ot_member, dpp::permissions::p_send_messages, 0);
	this->controller->channel_edit(ch);
}