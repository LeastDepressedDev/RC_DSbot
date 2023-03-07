#include <iostream>
#include <dpp/dpp.h>
#include <functional>
#include <vector>

#pragma once
class command
{
public:
	std::string cmd;
	std::string desc;
	std::function<void (const dpp::message_create_t& event, const std::vector<std::string> args)> realise;
	command(const std::string cmd, const std::string desc, 
		std::function<void (const dpp::message_create_t& event, const std::vector<std::string> args)> rl) {
		this->cmd = cmd;
		this->desc = desc;
		this->realise = rl;
	}
};

