#include "fw.h"
#include <vector>
#include <filesystem>
#include <direct.h>
#include <fstream>

std::map<std::string, std::string> fw::read(const char* path) {
	std::ifstream f;
	f.open(path);
	if (!f.is_open()) {
		std::ofstream of;
		of.open(path);
		of.close();
		f.open(path);
	}

	char buf;
	std::string line;
	while (f.read(&buf, sizeof(buf)))
	{
		line += buf;
	}

	std::map<std::string, std::string> set = std::map<std::string, std::string>();
	std::string s1 = "", s2 = "";
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '\n') {
			continue;
		}
		else if (line[i] == '@') {
			i++;
			for (i; i < line.length(); i++) {
				if (line[i] == '\n') {
					continue;
				}
				else if (line[i] == ';') {
					set[s1] = s2;
					s1 = ""; s2 = "";
					break;
				}
				else {
					s2 += line[i];
				}
			}
		}
		else {
			s1 += line[i];
		}
	}

	return set;
}

void fw::upt(const char* path, std::map<std::string, std::string> set) {
	std::ofstream of;
	of.open(path);
	std::string lines;
	for (std::pair<std::string, std::string> pr : set) {
		lines += pr.first + "@" + pr.second + ";\n";
	}

	of << lines;
	of.close();
}

bool fw::ex(std::string path) {
	std::ifstream fs;
	fs.open(path);
	if (fs.is_open()) {
		fs.close();
		return true;
	}
	else {
		return false;
	}
}