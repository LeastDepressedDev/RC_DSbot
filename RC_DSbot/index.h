#if !defined(_MSC_VER) || (_MSC_VER < 1929)
	#error "This template is only for Microsoft Visual C++ 2019 and later. To build a D++ bot in Visual Studio Code, or on any other platform or compiler please use https://github.com/brainboxdotcc/templatebot"
#endif

#if (!defined(_MSVC_LANG) || _MSVC_LANG < 201703L)
	#error "D++ bots require C++17 or later. Please enable C++17 under project properties."
#endif

#if !defined(DPP_WIN_TEMPLATE) && !defined(DPP_CI)
	#error "You must compile this template using its .sln file. You cannot just double click the .cpp file and compile it on its own. Ensure you checked out the full source code of the template!"
#endif

#pragma once

#include <iostream>
#include <dpp/dpp.h>
#include "LvlGrabber.h"

inline struct {
	const std::string TOKEN = "MTA3NjU5ODAxNDkwNDM4MTU3Mg.Gb_pgl.Q29hS2yzVmz_8u7lQ663SE2c49KaxDxYthbUjk";
	bool cpl = false;
	dpp::cluster* BOT;
	const uint16_t INTS = dpp::i_default_intents | dpp::i_privileged_intents;
	const std::string D_HOOK_URL = "https://discord.com/api/webhooks/1077557628760240189/19kfP3DyX3krv0ukiNiFymoX1Htv_DlqtRI-bAm1-c9gYT9zZVj4Quf_74kP-7FCQZX4";
	dpp::webhook DEBUG_HOOK;

	LvlGrabber* LVL;

	const std::string bot_name = "RC bot";
	const std::string version = "0.1a";
	const std::string dev = "Sirtage";
} APP;
