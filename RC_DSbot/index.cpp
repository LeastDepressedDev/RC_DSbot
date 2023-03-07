#include "index.h"
#include "inst.h"
#include <fstream>
#include <vector>
#include <dpp/dpp.h>
#include "string_proc.h"
#include "assets.h"

;

void wrap() {
    dpp::cluster* bot = APP.BOT;

    /*bot->on_slashcommand([](const dpp::slashcommand_t& event) {
        std::string str = event.command.get_command_name();
        for (command cmd : commands) {
            if (cmd.cmd == str) {
                cmd.realise(event);
            }
        }
        });*/

    bot->on_message_create([](const dpp::message_create_t& event) {
        std::string cont = event.msg.content;
        
        if (str_proc::start_with(cont, COM_PROC_DATA.call) && cont.length() > 0) {
            std::vector<std::string> agg(0);
            std::string builder = "";
            bool chained = false;
            for (int i = COM_PROC_DATA.call.length(); i < cont.length(); i++) {
                if (cont[i] == ' ' && !chained) {
                    agg.push_back(builder);
                    builder = "";
                }
                else if (cont[i] == '"') {
                    chained = !chained;
                }
                else {
                    builder += cont[i];
                }
            }
            agg.push_back(builder);


            for (command cmd : commands) {
                if (cmd.cmd == agg[0]) {;
                    std::async(std::launch::async, [cmd, event, agg] {cmd.realise(event, agg); });
                    return;
                }
            }

            APP.BOT->message_create(dpp::message(event.msg.channel_id, "Command not found."));
        }
        });

    bot->on_button_click([](const dpp::button_click_t& event) {
        event.reply();
        //event.reply(event.custom_id.substr(0, 3));
        if (event.custom_id.substr(0, 3) == "TTE") {
            tte::button_ofc(event.command.usr.id ,event.custom_id);
        }
        });

   /*bot->on_ready([bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
           
        }
        });*/
}

int8_t wake_rsc() {
    std::ifstream f;
    f.open(rsc::PC_FNAME);
    if (!f.is_open()) {
        return rsc::e_code::rsc_not_found;
    }

    char buf[256];
    std::string str = "";
    while (f.readsome(buf, sizeof(buf)))
    {
        str += buf;
    }

    try {
        std::string p1 = "", p2 = "";
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '@') {
                i++;
                for (i; i < str.length(); i++) {
                    if (str[i] == ';') {
                        rsc::m_keys[p1] = p2;
                        p1 = "";
                        p2 = "";
                        break;
                    }
                    else {
                        p2 += str[i];
                    }
                }
            }
            else {
                p1 += str[i];
            }
        }
    }
    catch (_exception e) {
        return rsc::e_code::rsc_error;
    }

    return rsc::e_code::rsc_loaded;
}

void create_rsc() {
    std::ofstream mf;
    mf.open(rsc::PC_FNAME);
    mf.close();
}

int main()
{
    APP.BOT = new dpp::cluster(APP.TOKEN, APP.INTS);

    APP.BOT->on_log(dpp::utility::cout_logger());

    APP.LOGGER = new ulogger();
    APP.LOGGER->prefix = "RC_DS";
    APP.LOGGER->sufix = "";
    APP.LOGGER->rTime = true;

    switch (wake_rsc()) {
    case -1:
        APP.LOGGER->log("[Err] Resource file not found. Creating new!");
        create_rsc();
    case -2:
        APP.LOGGER->log("[Err] Error occured in resource loading.");
        break;
    case 1:
        APP.LOGGER->log("[Inf] Resources loaded without any occurences");
        break;
    default:
        break;
    }
    wrap();
    //APP.cpl = true;

    APP.DEBUG_HOOK = dpp::webhook(APP.D_HOOK_URL);

    APP.BOT->start(dpp::st_wait);

    return 0;
}
