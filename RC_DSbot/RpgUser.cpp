#include "rpg_ac.h"

dpp::snowflake rpg::User::get_uid()
{
	return this->user_id;
}

rpg::User::User(dpp::snowflake sv) {
	this->user_id = sv;
}