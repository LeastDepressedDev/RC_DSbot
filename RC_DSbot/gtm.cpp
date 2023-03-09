#include "gtm.h"
#include "string_proc.h"

std::vector<gtm::gtm_session*> gtm::sessions = std::vector<gtm::gtm_session*>(0);
std::vector<dpp::snowflake> gtm::active_players = std::vector<dpp::snowflake>(0);
std::vector<std::string> gtm::resources = std::vector<std::string>(0);

void gtm::__init(std::map<std::string, std::string> &loc) {
	for (std::pair<std::string, std::string> obj : loc) {
		if (obj.first.substr(0, 4) == "gtm_") {
			gtm::resources.push_back(obj.second);
		}
	}
}

bool gtm::regNewPlayer(dpp::snowflake id) {
	for (dpp::snowflake t_id : gtm::active_players) {
		if (id == t_id) {
			return false;
		}
	}
	gtm::active_players.push_back(id);
	return true;
}

bool gtm::unregPlayer(dpp::snowflake id) {
	for (int i = 0; i < gtm::active_players.size(); i++) {
		if (id == gtm::active_players[i]) {
			gtm::active_players.erase(gtm::active_players.begin()+i);
			return true;
		}
	}
	return false;
}

bool gtm::hasChannel(dpp::snowflake id) {
	for (int i = 0; i < gtm::sessions.size(); i++) {
		for (std::pair<dpp::snowflake, dpp::snowflake> pr : gtm::sessions[i]->linker) {
			if (id == pr.second) {
				return true;
			}
		}
	}
	return false;
}

void gtm::regGame(gtm::gtm_session* game) {
	for (int i = 0; i < game->ucount; i++) {
		gtm::regNewPlayer(game->u_list[i]);
	}
	gtm::sessions.push_back(game);
}

void gtm::closeGame(gtm::gtm_session game) {
	// Will create it later
}

gtm::gtm_session* gtm::getPlSes(dpp::snowflake id) {
	for (gtm::gtm_session* a_gtm : gtm::sessions) {
		for (int i = 0; i < a_gtm->ucount; i++) {
			if (a_gtm->u_list[i] == id) {
				return a_gtm;
			}
		}
	}
	return nullptr;
}

gtm::gtm_session* gtm::getChSes(dpp::snowflake id) {
	for (gtm::gtm_session* a_gtm : gtm::sessions) {
		for (std::pair<dpp::snowflake, dpp::snowflake> sv : a_gtm->linker) {
			if (sv.second == id) {
				return a_gtm;
			}
		}
	}
	return nullptr;
}