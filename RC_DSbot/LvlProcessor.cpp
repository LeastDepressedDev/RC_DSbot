#include "LvlProcessor.h"
#include <string>

UserStat leveling::proc(dpp::snowflake sv, std::string str) {
	std::string b1, b2;
	std::string buf;
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == ':') {
			b1 = buf;
			buf = "";
			i++;
			for (i; i < str.length(); i++) {
				buf += str[i];
			}
		}
		else {
			buf += str[i];
		}
	}
	b2 = buf;
	
	return { sv, (uint16_t) std::stoi(b1), (ULONG) std::stoi(b2) };
}

std::string leveling::comp(UserStat stat) {
	return std::to_string(stat.lvl) + ":" + std::to_string(stat.pts);
}

ULONG leveling::req(uint16_t t) {
	return pow(2.71828182846, t / 20) + 10;
}