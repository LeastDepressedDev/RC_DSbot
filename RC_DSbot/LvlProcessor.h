#pragma once
#include <dpp/dpp.h>

typedef struct {
	dpp::snowflake uid;
	uint16_t lvl;
	ULONG pts;
} UserStat;

namespace leveling {
	extern UserStat proc(dpp::snowflake, std::string);
	extern std::string comp(UserStat);
}