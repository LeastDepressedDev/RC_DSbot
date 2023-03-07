#include "ChessHolder.h"

std::map<dpp::snowflake, ChessPartion*> Chess::partions = std::map<dpp::snowflake, ChessPartion*>();

bool Chess::is_chess_partion(dpp::snowflake cid) {
	for (std::pair<dpp::snowflake, ChessPartion*> pr : Chess::partions) {
		if (pr.first == cid) {
			return true;
		}
	}
	return false;
}