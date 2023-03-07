#include "tte.h"

std::vector<tte::tte_session*> tte::sessions = std::vector<tte::tte_session*>(0);

void tte::add(tte_session* ses) {
	tte::sessions.push_back(ses);
	ses->start();
}

void tte::enclose(std::pair<dpp::snowflake, dpp::snowflake> id) {
	std::vector<tte_session*> a_ses(tte::sessions.size()-1);
	int i = 0;
	for (tte::tte_session* ses: tte::sessions) {
		if (ses->p1 == id.first && ses->p2 == id.second) {
			delete ses;
			continue;
		}
		a_ses[i] = ses;
		i++;
	}
	tte::sessions = a_ses;
}

void tte::button_ofc(dpp::snowflake cl, std::string npc) {
	for (tte::tte_session* ses : tte::sessions) {
		if (ses->p1 == cl || ses->p2 == cl) {
			ses->act(cl, npc[4] - '0', npc[6] - '0');
			return;
		}
	}
}