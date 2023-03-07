#pragma once
#include <iostream>
#include "index.h"'
#include <dpp/dpp.h>

extern enum chess_figure {
	empty = 0,
	king = 1,
	queen = 2,
	pawn = 3,
	knight = 4,
	bishop = 5,
	rook = 6
};

extern enum chess_side {
	b_none,
	b_white,
	b_black
};

class ChessPartion {
public:
	dpp::snowflake channel_id;
	dpp::snowflake guild_id;
	std::pair<dpp::snowflake, dpp::snowflake> players;
	dpp::snowflake board_msg_id;
	bool started();
	void start();
	bool turn(std::pair<int, int> from, std::pair<int, int> to);
	bool turn(int from_x, int from_y, int to_x, int to_y);
	ChessPartion(dpp::snowflake g_id, dpp::snowflake user1, dpp::snowflake user2);
private:
	std::vector<std::vector<std::pair<chess_side, chess_figure>>> desk;
	std::string build_desk();
	char c_from_set(std::pair<chess_side, chess_figure>);
	void build_def_desk();
	bool turned = false;

	bool color_reversed = false;
	bool strd = false;
};