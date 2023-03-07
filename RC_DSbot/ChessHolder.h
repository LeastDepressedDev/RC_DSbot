#include <iostream>
#include "ChessPartion.h"
#include "index.h"
#include <map>


#pragma once
namespace Chess {
	extern std::map<dpp::snowflake, ChessPartion*> partions;

	extern bool is_chess_partion(dpp::snowflake channel_id);
}