#include "gtm_session.h"
#include "g_rand.h"
#include "gtm.h"
#include "index.h"

gtm::gtm_session::gtm_session(size_t sz_t, dpp::snowflake* p_uList) {
	this->ucount = sz_t;
	this->turn = i_rand(0, sz_t);
	this->u_list = p_uList;
}

void gtm::gtm_session::start() {
	this->controller = new dpp::cluster(APP.TOKEN, APP.INTS);
	this->ch_id = this->spec_ch();
}

dpp::snowflake gtm::gtm_session::spec_ch() {
	dpp::channel ch = dpp::channel();
	ch.set_type(dpp::channel_type::CHANNEL_TEXT);
	ch.name = "chess session N" + std::to_string(gtm::sessions.size());
	ch.guild_id = this->guild_id;
	ch.add_permission_overwrite(this->guild_id, dpp::overwrite_type::ot_role, 0, dpp::permissions::p_view_channel);

	for (int i = 0; i < this->ucount; i++) {
		ch.add_permission_overwrite(this->u_list[i], dpp::overwrite_type::ot_member, dpp::permissions::p_view_channel, 0);
	}

	
}