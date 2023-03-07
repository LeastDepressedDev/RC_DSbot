#include "ulogger.h"


ulogger::ulogger() {
	this->buf = std::vector<std::string>(0);
	this->prefix = "Ulog";
	this->sufix = ";";
	this->spoint = std::chrono::system_clock::now();
}

ulogger ulogger::operator<< (char* segment) {
	this->buf.push_back(segment);
	return *this;
}

void ulogger::log(const char* toLog) {
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::cout << ((this->rTime) ? (std::ctime(&end_time)) : "") << "@" <<
		((this->prefix.length() > 0) ? ("[" + this->prefix + "] ") : "") << toLog << sufix << std::endl;
}

void ulogger::push_buf() {
	for (std::string str : this->buf) {
		this->log(str.c_str());
	}
}

void ulogger::flush() {
	this->buf.clear();
}