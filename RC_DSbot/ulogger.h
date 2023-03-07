#pragma once
#include <time.h>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>

#pragma warning(disable : 4996)

class ulogger {
public:
	std::string prefix;
	std::string sufix;
	bool rTime = false;
	ulogger();

	ulogger operator<< (char*);

	void log(const char*);
	void push_buf();

	void flush();
private:	
	std::chrono::system_clock::time_point spoint;
	std::vector<std::string> buf;
};