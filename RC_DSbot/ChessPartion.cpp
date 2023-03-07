#include "ChessPartion.h"
#include "ChessHolder.h"
#include "g_rand.h"
#include "inst.h"

ChessPartion::ChessPartion(dpp::snowflake g_id, dpp::snowflake user1, dpp::snowflake user2) {
	this->guild_id = g_id;
	std::pair<dpp::snowflake, dpp::snowflake> pr(user1, user2);
	this->players = pr;
	std::vector<std::vector<std::pair<chess_side, chess_figure>>> revec(8);
	for (int i = 0; i < 8; i++) {
		std::vector<std::pair<chess_side, chess_figure>> semi_vec(8);
		for (int j = 0; j < 8; j++) {
			semi_vec[j] = std::pair<chess_side, chess_figure>(chess_side::b_none, chess_figure::empty);
		}
		revec[i] = semi_vec;
	}
	this->desk = revec;
}

bool ChessPartion::started() {
	return this->strd;
}

void ChessPartion::start() {
	dpp::component but = dpp::component();
	but.set_label("X");
	but.set_type(dpp::cot_button);
	but.set_style(dpp::component_style::cos_primary);

	dpp::cluster bot(APP.TOKEN, APP.INTS);

	this->color_reversed = b_rand();
	std::string w_line = "";
	if (color_reversed) {
		this->turned = true;
		w_line += "<@" + std::to_string(this->players.second) + "> - is playing white\n";
		w_line += "<@" + std::to_string(this->players.first) + "> - is playing black";
	}
	else {
		this->turned = false;
		w_line += "<@" + std::to_string(this->players.first) + "> - is playing white\n";
		w_line += "<@" + std::to_string(this->players.second) + "> - is playing black";
	}

	bot.message_create(dpp::message(this->channel_id, w_line));

	this->build_def_desk();

	dpp::message desk_msg(this->channel_id, this->build_desk());
	this->board_msg_id = bot.message_create_sync(desk_msg).id;
	this->strd = true;
}

void ChessPartion::build_def_desk() {
	for (int i = 0; i < 8; i++) {
		this->desk[1][i] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::pawn);
	}
	this->desk[0][0] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::rook);
	this->desk[0][7] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::rook);
	this->desk[0][1] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::knight);
	this->desk[0][6] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::knight);
	this->desk[0][2] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::bishop);
	this->desk[0][5] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::bishop);
	this->desk[0][3] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::queen);
	this->desk[0][4] = std::pair<chess_side, chess_figure>(chess_side::b_black, chess_figure::king);

	for (int i = 0; i < 8; i++) {
		this->desk[6][i] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::pawn);
	}
	this->desk[7][0] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::rook);
	this->desk[7][7] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::rook);
	this->desk[7][1] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::knight);
	this->desk[7][6] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::knight);
	this->desk[7][2] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::bishop);
	this->desk[7][5] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::bishop);
	this->desk[7][3] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::queen);
	this->desk[7][4] = std::pair<chess_side, chess_figure>(chess_side::b_white, chess_figure::king);
}

char ChessPartion::c_from_set(std::pair<chess_side, chess_figure> q) {
	if (q.first == chess_side::b_none) {
		return '□';
	} 
	else if (q.first == chess_side::b_white) {
		if (q.second == chess_figure::king) {
			return '♔';
		}
		else if (q.second == chess_figure::queen) {
			return '♕';
		}
		else if (q.second == chess_figure::knight) {
			return '♘';
		}
		else if (q.second == chess_figure::bishop) {
			return '♗';
		}
		else if (q.second == chess_figure::rook) {
			return '♖';
		}
		else if (q.second == chess_figure::pawn) {
			return '♙';
		}
		else {
			return 'E';
		}
	}
	else if (q.first == chess_side::b_black) {
		if (q.second == chess_figure::king) {
			return '♚';
		}
		else if (q.second == chess_figure::queen) {
			return '♛';
		}
		else if (q.second == chess_figure::knight) {
			return '♞';
		}
		else if (q.second == chess_figure::bishop) {
			return '♝';
		}
		else if (q.second == chess_figure::rook) {
			return '♜';
		}
		else if (q.second == chess_figure::pawn) {
			return '♟';
		}
		else {
			return 'E';
		}
	}
}

std::string ChessPartion::build_desk() {
	std::string line = "";
	line += "O|A      B      C      D      E      F      G      H\n-+-----------------------\n";
	for (int i = 0; i < 8; i++) {
		std::string semi = std::to_string(i + 1) + "|";
		for (int j = 0; j < 8; j++) {
			semi += this->c_from_set(this->desk[i][j]) + "      ";
		}
		semi += "\n  |\n";
		line += semi;
	}
	std::cout << line << std::endl;
	return line;
}