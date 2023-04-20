#pragma once
#include <iostream>
#include "command.h"
#include "index.h"
#include "Auds.h"
#include "tte.h"
#include "ChessHolder.h"
#include "ad_handler.h"
#include "assets.h"
#include "gtm.h"
#include <string>
#include <vector>
#include <thread>

const struct {
	const std::string call = "&&";
} COM_PROC_DATA;

const std::vector<command> commands = {
	command("info", "Main information", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		dpp::embed e = dpp::embed();
		e.color = dpp::colors::green;
		std::string lines = "Name: " + APP.bot_name + "\n";
		lines += "Version: " + APP.version + "\n";
		lines += "Author: " + APP.dev + "\n";
		e.add_field("Information about this shit)", lines);
		event.reply(dpp::message(event.msg.channel_id, e));
	}),
	command("spam", "I dont wanna comment this", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() >= 3) {
			for (int i = 0; i < std::stoi(args[2]); i++) {
				APP.BOT->message_create(dpp::message(event.msg.channel_id, args[1]));
			}
		}
		else {
			event.reply("Not enough arguments");
		}
	}),
	command("help", "Just help command", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		dpp::embed e = dpp::embed();
		e.color = dpp::colors::aquamarine;
		std::string lineBuilder = "";
		for (command cmd : commands) {
			lineBuilder += COM_PROC_DATA.call + cmd.cmd + ": " + cmd.desc + "\n";
		}

		e.add_field("Helping you!", lineBuilder);

		event.reply(dpp::message(event.msg.channel_id, e));
	}),
	command("clear", "Cleaning while you sleeping", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			int val = std::stoi(args[1]);
			dpp::channel ch = APP.BOT->channel_get_sync(event.msg.channel_id);
			dpp::message_map mp = APP.BOT->messages_get_sync(ch.id, ch.last_message_id, 0, val+1, val+1);
			std::vector<dpp::snowflake> vec(0);
			for (std::pair<dpp::snowflake, dpp::message> sw : mp) {
				vec.push_back(sw.first);
			}
			APP.BOT->message_delete_bulk_sync(vec, ch.id);
			APP.BOT->message_create(dpp::message(event.msg.channel_id, "Regular Clear completed."));
		}
	}),
	command("lclear", "Long clear for big amount of messages",[](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			int val = std::stoi(args[1]);
			dpp::channel ch = APP.BOT->channel_get_sync(event.msg.channel_id);
			dpp::message_map mp = APP.BOT->messages_get_sync(ch.id, ch.last_message_id, 0, val + 1, val + 1);
			for (std::pair<dpp::snowflake, dpp::message> sw : mp) {
				APP.BOT->message_delete(sw.first, ch.id);
			}
			APP.BOT->message_create(dpp::message(event.msg.channel_id, "Long clear completed."));
		}
	}),
	command("test", "Command for testing some shit",[](const dpp::message_create_t& event, const std::vector<std::string> args) {
		std::string toU = "";
		for (std::pair<std::string, std::string> pr : rsc::m_keys) {
			toU += pr.first + " : " + pr.second + '\n';
		}

		APP.BOT->message_create(dpp::message(event.msg.channel_id, toU));
	}),
	command("avatar", "Get avatar of sender",[](const dpp::message_create_t& event, const std::vector<std::string> args) {
		std::string url;
		dpp::embed e = dpp::embed();
		if (args.size() > 1) {
			dpp::user_identified usr = APP.BOT->user_get_sync(args[1].substr(2, args[1].length()-2));
			url = usr.get_avatar_url();

			e.add_field("Avatar of " + usr.username, url);
		}
		else {
			url = event.msg.author.get_avatar_url();
			e.add_field("Your avatar", url);
		}
		
		e.color = dpp::colors::bismark;
		dpp::embed_image img = dpp::embed_image();
		img.url = url;
		e.image = img;

		APP.BOT->message_create(dpp::message(event.msg.channel_id, e));
	}),
	command("connect", "Piece of something big", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			if (event.from->is_connected()) {
				event.from->disconnect_voice(event.msg.guild_id);
			}
			event.from->connect_voice(event.msg.guild_id, args[1]);
			//event.from->get_voice(event.msg.guild_id)->voiceclient->send_audio_opus();
		}
	}),
	command("disconnect", "Disconnect xd", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (event.from->is_connected()) {
			event.from->disconnect_voice(event.msg.guild_id);
		}
	}),
	command("audio", "Audio controller", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			if (args[1] == "help") {
				dpp::embed tab = dpp::embed();
				tab.add_field("Audio commands", "help - helping you\nplay - playing with you\nstop - stopping scary noizes");

				event.reply(dpp::message(event.msg.channel_id, tab));
			}
			else if (args[1] == "play") {
				if (event.from->is_connected()) {
					if (testSound.cor) {
						event.from->get_voice(event.msg.guild_id)->voiceclient->send_audio_raw((uint16_t*)testSound.sound(), testSound.sz());
					}
					else {
						event.reply("Error in path or smth like that");
					}
				}
				else {
					event.reply("Bot is not connected");
				}
			}
			else if (args[1] == "stop") {
				if (event.from->is_connected()) {
					event.from->get_voice(event.msg.guild_id)->voiceclient->stop_audio();
				}
				else {
					event.reply("Bot is not connected");
				}
			}
		}
	}),
	/*command("chess", "Chess, just chess", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			dpp::snowflake p1 = event.msg.author.id;
			dpp::snowflake p2 = args[1].substr(2, args[1].length() - 2);
			ChessPartion* part = new ChessPartion(event.msg.guild_id, p1, p2);
			
			dpp::channel ch = dpp::channel();
			ch.set_type(dpp::channel_type::CHANNEL_TEXT);
			ch.name = "chess session N" + std::to_string(Chess::partions.size());
			ch.guild_id = part->guild_id;

			ch.add_permission_overwrite(part->guild_id, dpp::overwrite_type::ot_role, 0, dpp::permissions::p_view_channel);
			ch.add_permission_overwrite(part->players.first, dpp::overwrite_type::ot_member, dpp::permissions::p_view_channel, 0);
			ch.add_permission_overwrite(part->players.second, dpp::overwrite_type::ot_member, dpp::permissions::p_view_channel, 0);

			dpp::snowflake id = APP.BOT->channel_create_sync(ch).id;
			part->channel_id = id;
			Chess::partions[id] = part;

			part->start();
		}
		else {
			event.reply("Not enough arguments");
		}
	}),*/
	command("tte", "Just a simple Tic Tac Toe", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			if (args[1] == "create") {
				if (args.size() > 2) {
					dpp::snowflake p1 = event.msg.author.id;
					dpp::snowflake p2 = args[2].substr(2, args[2].length() - 2);

					for (tte::tte_session* s : tte::sessions) {
						if (s->p1 == p1 || s->p2 == p1 || s->p1 == p2 || s->p2 == p2) {
							event.reply(dpp::message(event.msg.channel_id, "You already have an active tte session!"));
							return;
						}
					}

					tte::add(new tte::tte_session(std::pair<dpp::snowflake, dpp::snowflake>(event.msg.guild_id, event.msg.channel_id), p1, p2));
				}
				else {
					event.reply(dpp::message(event.msg.channel_id, "Not enough arguments for `create` command!"));
				}
			}
			else if (args[1] == "leave") {
				dpp::snowflake p = event.msg.author.id;

				for (tte::tte_session* s : tte::sessions) {
					if (s->p1 == p || s->p2 == p) {
						s->leave(p);
						return;
					}
				}

				event.reply(dpp::message(event.msg.channel_id, "You have no active sessions!"));
			}
			else {
				event.reply(dpp::message(event.msg.channel_id, "You can use only `create` and `leave` here!"));
			}
		}
	}),
	command("gtm", "Guess the meme game", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		event.reply(dpp::message("this command is temporary disabled due to my laziness!"));
		/*if (args.size() > 1) {

		}
		else {
			event.reply(dpp::message(event.msg.channel_id, gtm::sDescr));
		}*/
	}),
	command("lvl", "Main level command", [](const dpp::message_create_t& event, const std::vector<std::string> args) {
		if (args.size() > 1) {
			if (args[1] == "msv") {
				APP.LVL->upt();
				return;
			}
		}

		dpp::embed embd;
		embd.color = dpp::colors::rallyart_crimson;
		embd.title = "";
	})
};