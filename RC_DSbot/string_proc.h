#include <iostream>
#include <vector>

#pragma once
namespace str_proc {
	//bool compare(std::string s1, std::string s2);
	extern bool start_with(std::string s1, std::string s2);
	extern bool contains(std::string str, std::string segment);
	extern std::vector<std::string> vec_split(std::string str, char c_sep);
};