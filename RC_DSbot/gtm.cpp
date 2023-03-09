#include "gtm.h"

std::vector<gtm::gtm_session> gtm::sessions = std::vector<gtm::gtm_session>(0);
std::vector<dpp::snowflake> gtm::active_players = std::vector<dpp::snowflake>(0);

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
		for (std::pair<dpp::snowflake, dpp::snowflake> pr : gtm::sessions[i].linker) {
			if (id == pr.second) {
				return true;
			}
		}
	}
	return false;
}

void gtm::regGame(gtm::gtm_session game) {
	gtm::sessions.push_back(game);
}

void gtm::closeGame(gtm::gtm_session game) {
	// Will create it later
}