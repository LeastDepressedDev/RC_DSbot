#include "tte_session.h"
#include "g_rand.h"
#include "tte.h"
#include "index.h"

tte::tte_session::tte_session(std::pair<dpp::snowflake, dpp::snowflake> pos, dpp::snowflake p1, dpp::snowflake p2) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			this->desk[i][j] = "[ ]";
		}
	}
	this->position = pos;
	this->p1 = p1;
	this->p2 = p2;

	this->controller = new dpp::cluster(APP.TOKEN, APP.INTS);
}

void tte::tte_session::start() {
	

	this->reversed = b_rand();
	std::string w_line = "";
	if (reversed) {
		w_line += "<@" + std::to_string(this->p1) + "> - is playing cross\n";
		w_line += "<@" + std::to_string(this->p2) + "> - is playing cirlces";
		this->turn_p = this->p1;
		this->turn = true;
	}
	else {
		w_line += "<@" + std::to_string(this->p2) + "> - is playing cross\n";
		w_line += "<@" + std::to_string(this->p1) + "> - is playing circles";
		this->turn_p = this->p2;
		this->turn = false;
	}

	dpp::message msg(this->position.second, w_line);

	dpp::component but_sample = dpp::component();
	but_sample.set_type(dpp::component_type::cot_button);
	but_sample.set_label("[ ]");
	but_sample.set_style(dpp::component_style::cos_primary);


	for (int i = 0; i < 3; i++) {
		dpp::component line = dpp::component();
		for (int j = 0; j < 3; j++) {
			line.add_component(but_sample.set_id("TTE_" + std::to_string(i) + "_" + std::to_string(j)));
		}
		msg.add_component(line);
	}

	this->control_msg_id = this->controller->message_create_sync(msg).id;
}

void tte::tte_session::update() {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			std::string val = this->desk[i][j];
			if (val != "[ ]") {
				msg.components[i].components[j].set_disabled(true);
			}
			msg.components[i].components[j].set_label(val);
		}
	}

	this->controller->message_edit(msg);

	char s;
	if (turn) {
		if (reversed) s = 'X';
		else s = 'O';
	}
	else {
		if (reversed) s = 'O';
		else s = 'X';
	}

	bool isw = false;
	for (int i = 0; i < 3; i++) {
		if ((s == this->desk[i][0][0] && s == this->desk[i][1][0] && s == this->desk[i][2][0]) ||
			(s == this->desk[0][i][0] && s == this->desk[1][i][0] && s == this->desk[2][i][0])) {
			isw = true;
		}
	}
	if ((s == this->desk[0][0][0] && s == this->desk[1][1][0] && s == this->desk[2][2][0]) ||
		(s == this->desk[0][2][0] && s == this->desk[1][1][0] && s == this->desk[2][0][0])) {
		isw = true;
	}

	if (isw) {
		this->swp_player();
		this->outline(s);
		this->end(this->turn_p);
		return;
	}
	else {
		if (this->i_turn_c >= 9) {
			this->end();
			return;
		}
		this->turn = !this->turn;
		this->unfreeze();
	}
}

void tte::tte_session::act(dpp::snowflake sender, int x, int y) {
	if (this->ended || this->frozen) return;
	this->freeze();
	if (this->turn_p == sender) {
		if (this->desk[x][y] == "[ ]") {
			if (turn) {
				if (reversed) this->desk[x][y] = "X";
				else this->desk[x][y] = "O";
			}
			else {
				if (reversed) this->desk[x][y] = "O";
				else this->desk[x][y] = "X";
			}
		}
		else {
			this->controller->message_create(dpp::message(this->position.second, "<@" + std::to_string(sender) + ">! You can't act like this"));
			this->unfreeze();
			return;
		}
	}
	else {
		this->controller->message_create(dpp::message(this->position.second, "<@" + std::to_string(sender) + ">! Ther's not your turn."));
		this->unfreeze();
		return;
	}

	this->swp_player();
	this->i_turn_c++;
	this->update();
}

void tte::tte_session::leave(dpp::snowflake user) {
	this->controller->message_create(dpp::message(this->position.second, "<@" + std::to_string(user) + "> left the game."));
	if (this->p1 == user) {
		this->end(p2);
	}
	else if (this->p2 == user) {
		this->end(p1);
	}
	else {
		std::string line = "Something went wrong in session - " + this->p1 + ':' + this->p2;
		std::cout << line << std::endl;
		dpp::cluster bot("");
		bot.execute_webhook(APP.DEBUG_HOOK, line);
	}
}

void tte::tte_session::end(dpp::snowflake usr) {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	msg.content = "**Wow!** <@" + std::to_string(usr) + "> won the game!";
	this->controller->message_edit(msg);
	this->ended = true;
	tte::enclose(std::pair<dpp::snowflake, dpp::snowflake>(this->p1, this->p2));
}

void tte::tte_session::end() {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	msg.content = "**Draw!**";
	this->controller->message_edit(msg);
	this->ended = true;
	tte::enclose(std::pair<dpp::snowflake, dpp::snowflake>(this->p1, this->p2));
}

void tte::tte_session::freeze() {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->desk[i][j] == "[ ]") {
				msg.components[i].components[j].set_disabled(true);
			}
		}
	}
	this->controller->message_edit(msg);
	frozen = true;
}
void tte::tte_session::unfreeze() {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->desk[i][j] == "[ ]") {
				msg.components[i].components[j].set_disabled(false);
			}
		}
	}
	this->controller->message_edit(msg);
	frozen = false;
}

void tte::tte_session::outline(char c) {
	dpp::message msg = this->controller->message_get_sync(this->control_msg_id, this->position.second);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (this->desk[i][j][0] == c) {
				msg.components[i].components[j].set_style(dpp::component_style::cos_success);
			}
		}
	}
	this->controller->message_edit(msg);
}

void tte::tte_session::swp_player() {
	if (this->turn_p == p1) this->turn_p = p2;
	else this->turn_p = p1;
}